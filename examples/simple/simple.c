#include <rtthread.h>
#include <rtdevice.h>
#include <stdio.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "simple.pb.h"

static void nanopb_simple(int argc,char *argv[])
{
    /* This is the buffer where we will store our message. */
    uint8_t buffer[128];
    size_t message_length;
    bool status;
    
    /* Encode our message */
    {
        /* Allocate space on the stack to store the message data.
         *
         * Nanopb generates simple struct definitions for all the messages.
         * - check out the contents of simple.pb.h!
         * It is a good idea to always initialize your structures
         * so that you do not have garbage data from RAM in there.
         */
        SimpleMessage message = SimpleMessage_init_zero;
        
        /* Create a stream that will write to our buffer. */
        pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
        
        /* Fill in the lucky number */
        message.lucky_number = 13;
        
        /* Now we are ready to encode the message! */
        status = pb_encode(&stream, SimpleMessage_fields, &message);
        message_length = stream.bytes_written;
        
        /* Then just check for any errors.. */
        if (!status)
        {
            rt_kprintf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
            return ;
        }
		rt_kprintf("Successfully encoded\n");
    }
    
    /* Now we could transmit the message over network, store it in a file or
     * wrap it to a pigeon's leg.
     */

    /* But because we are lazy, we will just decode it immediately. */
    
    {
        /* Allocate space for the decoded message. */
        SimpleMessage message = SimpleMessage_init_zero;
        
        /* Create a stream that reads from the buffer. */
        pb_istream_t stream = pb_istream_from_buffer(buffer, message_length);
        
        /* Now we are ready to decode the message. */
        status = pb_decode(&stream, SimpleMessage_fields, &message);
        
        /* Check for errors... */
        if (!status)
        {
            rt_kprintf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
            return;
        }
        
        /* Print the data contained in the message. */
        rt_kprintf("Your lucky number was %d!\n", (int)message.lucky_number);
    }
    
    return;
}
MSH_CMD_EXPORT(nanopb_simple, nanopb test);
