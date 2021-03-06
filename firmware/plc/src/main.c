#include "app_config.h"
#include "hal.h"
#include "io.h"
#include "locks.h"
#include "plc.h"
#include "tools.h"
#include "ui.h"
#include "uavcan_impl.h"

static void main_init();
static void main_task(void *pvParameters);

#define START(lbl, init_fun)                                                   \
	{                                                                      \
		ui_set_status(lbl);                                            \
		PRINTF(lbl " ... ");                                           \
		if (init_fun) {                                                \
			die(DEATH_INIT_FAILED);                                \
		} else {                                                       \
			PRINTF("OK\n");                                        \
		}                                                              \
	}

static void main_init()
{
	if (log_init()) {
		die(DEATH_INIT_FAILED);
	}
	PRINTF("\n");
	PRINTF("-----------------------------------------------------\n");
#ifdef WITH_CAN
	const char *uavcan_ver = " (UAVCAN v.0)";
#else
	const char *uavcan_ver = " (no CAN)";
#endif
	PRINTF("%s v. %d.%d%s\n\n", APP_NAME, APP_VERSION_MAJOR,
	       APP_VERSION_MINOR, uavcan_ver);

	START("locks", locks_init());
	START("ui", ui_init());
	START("io", io_init());
	// plc buffers must be initialized before UAVCAN is started
	START("plc", plc_init());
#ifdef WITH_CAN
	START("UAVCAN", uavcan2_init());
#endif
#ifdef WITH_WIFI
	START("wifi", wifi_init());
#endif
#ifdef WITH_MQTT
	START("mqtt", mqtt_init());
#endif

	PRINTF("-----------------------------------------------------\n");

	plc_set_state(PLC_STATE_RUNNING);
}

static void main_task(void *pvParameters)
{
	TickType_t delay = portMAX_DELAY;

	for (;;) {
		vTaskDelay(pdMS_TO_TICKS(delay));
	}
}

#ifdef ESP32
void app_main()
{
	main_init();
	main_task(NULL);
	die(DEATH_UNREACHABLE_REACHED);
}
#endif // ifdef ESP32
