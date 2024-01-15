#ifndef __ESPRESS_STREAM_API_H__
#define __ESPRESS_STREAM_API_H__

/* espress_stream APIs
 * 
 * - Function calling procedure.
 * 
 *     espress_stream_begin
 *        |
 *        |<--------------------------------------------------------------+
 *        |                                                               |
 *        +-------------------------------+...                            |
 *        |                               |                               |
 *        V                               V                               |
 *     espress_stream_begin_interface  espress_stream_begin_interface     |
 *        |                               :                               |
 *        |<----------------------------------------------------------+   |
 *        |                                                           |   |
 *        +-------------------------------+...                        |   |
 *        |(client session)               |(server session)           |   |
 *        V                               V                           |   |
 *     espress_stream_begin_session    espress_stream_begin_session   |   |
 *        |                               |                           |   |
 *     espress_stream_connect          espress_stream_connect_detect  |   |
 *        |                               |                           |   |
 *     espress_stream_connect_result      |                           |   |
 *        |                               |                           |   |
 *        |<------------------------------+                           |   |
 *        |                                                           |   |
 *        |<------------------------------+                           |   |
 *        |                               |                           |   |
 *        V                               |                           |   |
 *     espress_stream_send                |                           |   |
 *     espress_stream_flush               |                           |   |
 *     espress_stream_recv                |                           |   |
 *        |                               |                           |   |
 *        +-------------------------------+                           |   |
 *        |                                                           |   |
 *        V                                                           |   |
 *     espress_stream_disconnect                                      |   |
 *        |                                                           |   |
 *        V                                                           |   |
 *     espress_stream_disconnect_result                               |   |
 *        |                                                           |   |
 *        V                                                           |   |
 *     espress_stream_end_session                                     |   |
 *        |                                                           |   |
 *        +-----------------------------------------------------------+   |
 *        |                                                               |
 *        V                                                               |
 *     espress_stream_end_interface                                       |
 *        |                                                               |
 *        +---------------------------------------------------------------+
 *        |
 *        V
 *     espress_stream_end
 * 
 * - Async running.
 * 
 *     Different sessions are async runnable.
 *     In single session functions are not able to async run
 *     except for "espress_stream_wait_break" function.
 * 
 * - Return code.
 * 
 *     - == 0
 *         Complete in success.
 * 
 *     - > 0
 *         Finished in failure.
 *         In a session when a function return positive value in spite of right procedure,
 *         this session should be destroyed by "espress_stream_end_session" function.
 *         Reprocess from beginning session.
 * 
 *     - == -1
 *         Unfinished. Functions with timeout may return negative one.
 *         Retry run this function.
 */

#include "espress_stream/api_depend.h"

#define ESPRESS_STREAM_SESSION_HASH_SIZE (32)
#define ESPRESS_STREAM_MAX_NUM_POLL_SESSION (ESPRESS_STREAM_MAX_NUM_SESSION)

#define ESPRESS_STREAM_EVENT_CONNECT_RESULT (1 << 0)
#define ESPRESS_STREAM_EVENT_CONNECT_DETECT (1 << 1)
#define ESPRESS_STREAM_EVENT_DISCONNECT_RESULT (1 << 2)
#define ESPRESS_STREAM_EVENT_SEND (1 << 3)
#define ESPRESS_STREAM_EVENT_RECV (1 << 4)

struct espress_stream_session_ex_param {
	int (*send_connect_payload)(void *sid, int (*callback)(void *sid, void const *payload, unsigned payload_size, int is_rejected), void *param);
	void *send_connect_payload_param;
	int (*recv_connect_payload)(void *sid, int (*callback)(void *sid, void *payload, unsigned *payload_size), void *param);
	void *recv_connect_payload_param;
	int (*send_disconnect_payload)(void *sid, int (*callback)(void *sid, void const *payload, unsigned payload_size), void *param);
	void *send_disconnect_payload_param;
	int (*recv_disconnect_payload)(void *sid, int (*callback)(void *sid, void *payload, unsigned *payload_size), void *param);
	void *recv_disconnect_payload_param;
};

/** @brief Begin stack.
 * 
 * Begin espress_stream protocol stack.
 * 
 * @return
 *     == 0: Success.
 *     > 0 : Failure.
 */
int espress_stream_begin(void);

/** @brief Finish stack.
 * 
 * Finish espress_stream protocol stack.
 */
void espress_stream_end(void);

/** @brief Get copyright string.
 * 
 * @return
 *     Stored address of null terminated copyright string.
 */
char const *espress_stream_copyright(void);

/** @brief Tick time.
 * 
 * Get tick time in microsecond.
 * 
 * @return
 *     Tick time. It's only lower 32 bits are available. The unit is microsecond.
 */
unsigned espress_stream_time(void);

/** @brief Begin interface.
 * 
 * Begin interface. It is consists of a datagrum session (UDP socket when on linux).
 * 
 * @param[out] ifd
 *     Store address of interface ID (It's type is void *).
 * @param[in] address
 *     Stored address of an interface bind network address (include port number when it is IP address).
 * 
 * @return
 *     == 0: Success.
 *     > 0 : Failure.
 */
int espress_stream_begin_interface(void **ifd, struct sockaddr_storage const *address);

/** @brief Finish interface.
 * 
 * Finish interface.
 * 
 * @param[in] ifd
 *     Interface ID.
 */
void espress_stream_end_interface(void *ifd);

/** @brief Begin session.
 * 
 * Begin session. It is consists of a one connection.
 * In case of server session, the connection established session is used for transmission as it is.
 * When multiple connection acception in same time is required, begin multiple server session in same time.
 * 
 * @param[out] sid
 *     Store address of session ID (It's type is void *).
 * @param[in] ifd
 *     An interface ID that the session belong.
 * @param[in] is_server
 *     When this session is a server session, set it to non zero value.
 * @param[in] ex_param
 *     For future use. set NULL yet.
 * 
 * @return
 *     == 0: Success.
 *     > 0 : Failure.
 */
int espress_stream_begin_session(void **sid, void *ifd, int is_server, struct espress_stream_session_ex_param const *ex_param);

/** @brief Finish session.
 * 
 * Finish session.
 * 
 * @param[in] sid
 *     Session ID.
 */
void espress_stream_end_session(void *sid);

/** @brief Request to connect.
 * 
 * Request to connect (only for client session).
 * 
 * @param[in] sid
 *     Session ID.
 * @param[in] address
 *     Stored address of a destination network address (include port number when it is IP address).
 * 
 * @return
 *     == 0: Success.
 *     > 0 : Failure.
 */
int espress_stream_connect(void *sid, struct sockaddr_storage const *address);

/** @brief Confirm result of request to connect.
 * 
 * Confirm result of request to connect. It may be waiting for result.
 * It may be not able to get result ever. In this case user should rise timeout error.
 * This is used for only client.
 * 
 * @param[in] sid
 *     Session ID.
 * @param[in] timeout
 *     Timeout value. It's only lower 32 bits are available. The unit is microsecond.
 * 
 * @return
 *     == 0 : Success.
 *     == -1: Waiting for result yet.
 *     > 0  : Failure.
 */
int espress_stream_connect_result(void *sid, unsigned timeout);

/** @brief Confirm detection of incoming connect.
 * 
 * Confirm detection of incoming connect. It may be waiting for detection.
 * It may be not able to get detection ever. In this case user should rise timeout error.
 * This is used for only server.
 * 
 * @param[in] sid
 *     Session ID.
 * @param[in] timeout
 *     Timeout value. It's only lower 32 bits are available. The unit is microsecond.
 * 
 * @return
 *     == 0 : Success.
 *     == -1: Waiting for detection yet.
 *     > 0  : Failure.
 */
int espress_stream_connect_detect(void *sid, unsigned timeout);

/** @brief Request to disconnect.
 * 
 * Request to disconnect. The disconnect is to close the sending part of half session.
 * The receiving part of the other half session is able to active close by only the destination node.
 * 
 * @param[in] sid
 *     Session ID.
 * 
 * @return
 *     == 0: Success.
 *     > 0 : Failure.
 */
int espress_stream_disconnect(void *sid);

/** @brief Confirm result of request to disconnect.
 * 
 * Confirm result of request to disconnect. It may be waiting for result.
 * It may be not able to get result ever. In this case user should rise timeout error.
 * 
 * @param[in] sid
 *     Session ID.
 * @param[in] timeout
 *     Timeout value. It's only lower 32 bits are available. The unit is microsecond.
 * 
 * @return
 *     == 0 : Success.
 *     == -1: Waiting for result yet.
 *     > 0  : Failure.
 */
int espress_stream_disconnect_result(void *sid, unsigned timeout);

/** @brief Request to send data.
 * 
 * Request to send data. The all send request data may be not able to be accepted by timeout time.
 * When accepted send request data is nothing, the sent data size do not become zero but return value become negative one.
 * 
 * @param[in] sid
 *     Session ID.
 * @param[in] data
 *     Stored address of data.
 * @param[in,out] data_size
 *     Stored address of data size and store address of accepted send request data size.
 * @param[in] timeout
 *     Timeout value. It's only lower 32 bits are available. The unit is microsecond.
 * 
 * @return
 *     == 0 : Success.
 *     == -1: Waiting for acceptable state yet.
 *     > 0  : Failure.
 */
int espress_stream_send(void *sid, void const *data, unsigned *data_size, unsigned timeout);

/** @brief Request to flush data.
 * 
 * Request to flush data. When send request data is accepted,
 * the last remainder part of the data divided by packet payload size are not sent soon.
 * Requesting to flush is to force send this data.
 * When this function's return value is negative one,
 * this data is not sent yet but it would be sent as soon as possible.
 * 
 * @param[in] sid
 *     Session ID.
 * @param[in] timeout
 *     Timeout value. It's only lower 32 bits are available. The unit is microsecond.
 * 
 * @return
 *     == 0 : Success.
 *     == -1: Waiting for send complete yet.
 *     > 0  : Failure.
 */
int espress_stream_flush(void *sid, unsigned timeout);

/** @brief Request to receive data.
 * 
 * Request to receive data. When received data is nothing until timeout time,
 * the received data size do not become zero but return value become negative one.
 * When this function's return value is zero and received data size is zero,
 * it is mean that destination node's disconnect is detected.
 * 
 * @param[in] sid
 *     Session ID.
 * @param[out] data
 *     Store address of received data.
 * @param[in,out] data_size
 *     Stored address of data region size and store address of received data size.
 * @param[in] timeout
 *     Timeout value. It's only lower 32 bits are available. The unit is microsecond.
 * 
 * @return
 *     == 0 : Success.
 *     == -1: Waiting for incoming data yet.
 *     > 0  : Failure.
 */
int espress_stream_recv(void *sid, void *data, unsigned *data_size, unsigned timeout);

/** @brief Wait for any event.
 * 
 * Waits until the result of any function with a timeout parameter is available (i.e. return value is greater than or equal to zero).
 * To confirm which function is able to get result, see return value of each function called with zero timeout.
 * Even if this function finish in success, none function's result may be abailable.
 * In this case, repeat call this function.
 * You are able to choose event types which relate the function to wait for result.
 * 
 * | Event types                              | Functions                                 |
 * |------------------------------------------|-------------------------------------------|
 * | ESPRESS_STREAM_EVENT_CONNECT_RESULT      | espress_stream_connect_result             |
 * | ESPRESS_STREAM_EVENT_CONNECT_DETECT      | espress_stream_connect_detect             |
 * | ESPRESS_STREAM_EVENT_DISCONNECT_RESULT   | espress_stream_disconnect_result          |
 * | ESPRESS_STREAM_EVENT_SEND                | espress_stream_send, espress_stream_flush |
 * | ESPRESS_STREAM_EVENT_RECV                | espress_stream_recv                       |
 * 
 * One or more event types are selectable. Set event types concatenated with "|" to the event parameter.
 * Note that each events are set by change in state and reset by calling to the related function.
 * For example, the ESPRESS_STREAM_EVENT_SEND event is set by incleased free space in the send buffer and reset by calling to espress_stream_send function.
 * For this reason, even if it is possible to send the ESPRESS_STREAM_EVENT_SEND event may not be setting.
 * When sending data, instead of waiting for the event before sending the data, attempt to send the data and wait for the event if sending the data is pending.
 * Similar processing must be done for the ESPRESS_STREAM_EVENT_RECV event.
 * 
 * @param[in] sid
 *     Session ID.
 * @param[in] event
 *     Event types.
 * @param[in] timeout
 *     Timeout value. It's only lower 32 bits are available. The unit is microsecond.
 * 
 * @return
 *     == 0 : Success.
 *     == -1: Waiting for event yet.
 *     > 0  : Failure.
 */
int espress_stream_wait_any(void *sid, unsigned event, unsigned timeout);

/** @brief Break to waiting function
 * 
 * Break to waiting function. Apply this function to the function with timeout parameter.
 * The waiting function return soon because of timeout, when this function is called.
 * 
 * @param[in] sid
 *     Session ID.
 */
void espress_stream_wait_break(void *sid);

/** @brief Polling multiple session.
 * 
 * Detect that any espress_stream_wait_any function result for one or more sids will be success or failure at least one.
 * 
 * @param[in] sids
 *     Session IDs array.
 * @param[in] events
 *     Event types array.
 * @param[in] num
 *     Number of sids and events items.
 * @param[in] timeout
 *     Timeout value. It's only lower 32 bits are available. The unit is microsecond.
 * 
 * @return
 *     >= 0 : Number of detected sids. (Zero indicates timeout.)
 *     == -1: Failure.
 */
int espress_stream_poll(void * const *sids, unsigned const *events, unsigned num, unsigned timeout);

/** @brief Get source address.
 * 
 * Get source network address.
 * 
 * @param[in] sid
 *     Session ID.
 * @param[out] address
 *     Store address of a source network address.
 * 
 */
void espress_stream_src_address(void *sid, struct sockaddr_storage *address);

/** @brief Get destination address.
 * 
 * Get destination network address.
 * 
 * @param[in] sid
 *     Session ID.
 * @param[out] address
 *     Store address of a destination network address.
 * 
 */
void espress_stream_dst_address(void *sid, struct sockaddr_storage *address);

/** @brief Get connection ID.
 * 
 * Get connection ID value.
 * 
 * @param[in] sid
 *     Session ID.
 * 
 * @return
 *     Connection ID value. It's only lower 32 bits are available. The unit is microsecond.
 */
unsigned espress_stream_connection_id(void *sid);

/** @brief Get session HASH.
 * 
 * Get session HASH.
 * 
 * @param[in] sid
 *     Session ID.
 * @param[out] session_hash
 *     Store address of session hash bytes. Its region size is ESPRESS_STREAM_SESSION_HASH_SIZE bytes at least.
 */
void espress_stream_session_hash(void *sid, void *session_hash);

/** @brief Get buffered send data size.
 * 
 * Get buffered send data size.
 * 
 * @param[out] max_size
 *     Store address of maximum buffered send data size. If retrieving it is't needed, set this parameter to NULL.
 * 
 * @return
 *     Buffered send data size.
 */
unsigned espress_stream_buffered_send_size(void *sid, unsigned *max_size);

/** @brief Get buffered recv data size.
 * 
 * Get buffered recv data size.
 * 
 * @param[out] max_size
 *     Store address of maximum buffered recv data size. If retrieving it is't needed, set this parameter to NULL.
 * 
 * @return
 *     Buffered recv data size.
 */
unsigned espress_stream_buffered_recv_size(void *sid, unsigned *max_size);

/** @brief Get rtt.
 * 
 * Get round trip time value. For debugging use.
 * 
 * @param[in] sid
 *     Session ID.
 * 
 * @return
 *     Round trip time value. It's only lower 32 bits are available. The unit is microsecond.
 */
unsigned espress_stream_rtt(void *sid);

/** @brief Get rto.
 * 
 * Get retransmission timeout value. For debugging use.
 * 
 * @param[in] sid
 *     Session ID.
 * 
 * @return
 *     Retransmission timeout value. It's only lower 32 bits are available. The unit is microsecond.
 */
unsigned espress_stream_rto(void *sid);

/** @brief Get dac.
 * 
 * Get duplicate acknowledge count. For debugging use.
 * 
 * @param[in] sid
 *     Session ID.
 * 
 * @return
 *     Duplicate acknowledge count.
 */
unsigned espress_stream_dac(void *sid);

/** @brief Get cwnd.
 * 
 * Get congestion window size. For debugging use.
 * 
 * @param[in] sid
 *     Session ID.
 * 
 * @return
 *     Congestion window size. The unit is packet payload size.
 */
unsigned espress_stream_cwnd(void *sid);

/** @brief Get send timeout count.
 * 
 * Get send timeout count. For debugging use.
 * 
 * @param[in] sid
 *     Session ID.
 * 
 * @return
 *     Send timeout count.
 */
unsigned espress_stream_send_timeout_count(void *sid);

/** @brief Get fast retransmit count.
 * 
 * Get fast retransmit count. For debugging use.
 * 
 * @param[in] sid
 *     Session ID.
 * 
 * @return
 *     Fast retransmit count.
 */
unsigned espress_stream_fast_retransmit_count(void *sid);

#endif /* __ESPRESS_STREAM_API_H__ */
