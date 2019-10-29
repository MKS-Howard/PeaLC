/*
 * UAVCAN data structure definition for libcanard.
 *
 * Autogenerated, do not edit.
 *
 */
#include "automation/Values.h"
#include "canard.h"

#ifndef CANARD_INTERNAL_SATURATE
#define CANARD_INTERNAL_SATURATE(x, max) ( ((x) > max) ? max : ( (-(x) > max) ? (-max) : (x) ) );
#endif

#ifndef CANARD_INTERNAL_SATURATE_UNSIGNED
#define CANARD_INTERNAL_SATURATE_UNSIGNED(x, max) ( ((x) >= max) ? max : (x) );
#endif

#if defined(__GNUC__)
# define CANARD_MAYBE_UNUSED(x) x __attribute__((unused))
#else
# define CANARD_MAYBE_UNUSED(x) x
#endif

/**
  * @brief automation_Values_encode_internal
  * @param source : pointer to source data struct
  * @param msg_buf: pointer to msg storage
  * @param offset: bit offset to msg storage
  * @param root_item: for detecting if TAO should be used
  * @retval returns new offset
  */
uint32_t automation_Values_encode_internal(automation_Values* source,
  void* msg_buf,
  uint32_t offset,
  uint8_t CANARD_MAYBE_UNUSED(root_item))
{
    // Max Union Tag Value
    CANARD_ASSERT(source->union_tag <= 1);

    // Union Tag 1 bit
    canardEncodeScalar(msg_buf, offset, 1, (void*)&source->union_tag); // 1 bit
    offset += 1;

    if (source->union_tag == 0) {
    // Compound
    offset = automation_DigitalValues_encode_internal(&source->digital_values, msg_buf, offset, 0);
    }
    else if (source->union_tag == 1) {
    // Compound
    offset = automation_AnalogValues_encode_internal(&source->analog_values, msg_buf, offset, root_item);
    }

    return offset;
}

/**
  * @brief automation_Values_encode
  * @param source : Pointer to source data struct
  * @param msg_buf: Pointer to msg storage
  * @retval returns message length as bytes
  */
uint32_t automation_Values_encode(automation_Values* source, void* msg_buf)
{
    uint32_t offset = 0;

    offset = automation_Values_encode_internal(source, msg_buf, offset, 1);

    return (offset + 7 ) / 8;
}

/**
  * @brief automation_Values_decode_internal
  * @param transfer: Pointer to CanardRxTransfer transfer
  * @param payload_len: Payload message length
  * @param dest: Pointer to destination struct
  * @param dyn_arr_buf: NULL or Pointer to memory storage to be used for dynamic arrays
  *                     automation_Values dyn memory will point to dyn_arr_buf memory.
  *                     NULL will ignore dynamic arrays decoding.
  * @param offset: Call with 0, bit offset to msg storage
  * @retval new offset or ERROR value if < 0
  */
int32_t automation_Values_decode_internal(
  const CanardRxTransfer* transfer,
  uint16_t CANARD_MAYBE_UNUSED(payload_len),
  automation_Values* dest,
  uint8_t** CANARD_MAYBE_UNUSED(dyn_arr_buf),
  int32_t offset)
{
    int32_t ret = 0;

    // Get Union Tag
    ret = canardDecodeScalar(transfer, (uint32_t)offset, 1, false, (void*)&dest->union_tag); // 1
    if (ret != 1)
    {
        goto automation_Values_error_exit;
    }
    offset += 1;

    if (dest->union_tag == 0)
    {
    // Compound
    offset = automation_DigitalValues_decode_internal(transfer, payload_len, &dest->digital_values, dyn_arr_buf, offset);
    if (offset < 0)
    {
        ret = offset;
        goto automation_Values_error_exit;
    }
    }
    else if (dest->union_tag == 1)
    {
    // Compound
    offset = automation_AnalogValues_decode_internal(transfer, payload_len, &dest->analog_values, dyn_arr_buf, offset);
    if (offset < 0)
    {
        ret = offset;
        goto automation_Values_error_exit;
    }
    }
    return offset;

automation_Values_error_exit:
    if (ret < 0)
    {
        return ret;
    }
    else
    {
        return -CANARD_ERROR_INTERNAL;
    }
}

/**
  * @brief automation_Values_decode
  * @param transfer: Pointer to CanardRxTransfer transfer
  * @param payload_len: Payload message length
  * @param dest: Pointer to destination struct
  * @param dyn_arr_buf: NULL or Pointer to memory storage to be used for dynamic arrays
  *                     automation_Values dyn memory will point to dyn_arr_buf memory.
  *                     NULL will ignore dynamic arrays decoding.
  * @retval offset or ERROR value if < 0
  */
int32_t automation_Values_decode(const CanardRxTransfer* transfer,
  uint16_t payload_len,
  automation_Values* dest,
  uint8_t** dyn_arr_buf)
{
    const int32_t offset = 0;
    int32_t ret = 0;

    // Clear the destination struct
    for (uint32_t c = 0; c < sizeof(automation_Values); c++)
    {
        ((uint8_t*)dest)[c] = 0x00;
    }

    ret = automation_Values_decode_internal(transfer, payload_len, dest, dyn_arr_buf, offset);

    return ret;
}