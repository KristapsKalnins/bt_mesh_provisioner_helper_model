#ifndef BT_MESH_PROV_HELPER_SRV_H__
#define BT_MESH_PROV_HELPER_SRV_H__

#include <bluetooth/mesh/model_types.h>
#include <bluetooth/mesh/models.h>
#include <zephyr/sys/util.h>
#include <zephyr/bluetooth/mesh/cdb.h>
#include "prov_helper.h"

#define MODEL_SRV_ID   0x5677
#define BT_MESH_VND_MODEL_ID_PROV_HELPER_SRV MODEL_SRV_ID

#define BT_MESH_MODEL_PROV_HELPER_SRV(_helper_srv)                                  \
            BT_MESH_MODEL_VND_CB(COMPANY_ID,                                        \
                                 MODEL_SRV_ID,                                      \
                                 _bt_mesh_prov_helper_srv_opcode_list,              \
                                 &(_helper_srv)->pub,                               \
                                 BT_MESH_MODEL_USER_DATA(                           \
                                    struct bt_mesh_prov_helper_srv, _helper_srv),   \
                                 &_bt_mesh_prov_helper_srv_cb)

#define BT_MESH_PROV_HELPER_SRV_INIT(_handlers)                     \
        {   .handlers = _handlers,									\
            .pub = {.msg = NET_BUF_SIMPLE(BT_MESH_MODEL_BUF_LEN(    \
	        BT_MESH_PROV_HELPER_OP_NODEINFO,                        \
	        BT_MESH_PROV_HELPER_MSG_LEN_NODEINFO)) }                \
        }


struct bt_mesh_prov_helper_srv {
   	/** Model entry. */
	struct bt_mesh_model *model;
	/** Publish parameters. */
	struct bt_mesh_model_pub pub;
	/** Acknowledged message tracking. */
	struct bt_mesh_msg_ack_ctx ack_ctx;
	///** Collection of handler callbacks.
	//*
	//* @note Must point to memory that remains valid.
	//*/
	const struct bt_mesh_time_srv_handlers *handlers; 
};

struct bt_mesh_time_srv_handlers {

	void (*const prov_helper_message_appkey)(
		struct bt_mesh_prov_helper_srv* srv, struct bt_mesh_msg_ctx *ctx,
		struct net_buf_simple *buf);

	void (*const prov_helper_message_netkey)(
		struct bt_mesh_prov_helper_srv* srv, struct bt_mesh_msg_ctx *ctx,
		struct net_buf_simple *buf);

	void (*const prov_helper_message_nodeinfo)(
		struct bt_mesh_prov_helper_srv* srv, struct bt_mesh_msg_ctx *ctx,
		struct net_buf_simple *buf);
	
	void (*const prov_helper_message_addrinfo)(
		struct bt_mesh_prov_helper_srv* srv, struct bt_mesh_msg_ctx *ctx,
		struct net_buf_simple *buf);
};




extern const struct bt_mesh_model_op _bt_mesh_prov_helper_srv_opcode_list[];
extern const struct bt_mesh_model_cb _bt_mesh_prov_helper_srv_cb;

#endif /* BT_MESH_PROV_HELPER_SRV_H__ */