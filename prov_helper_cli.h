
#ifndef BT_MESH_PROV_HELPER_CLI_H__
#define BT_MESH_PROV_HELPER_CLI_H__

#include <bluetooth/mesh/model_types.h>
#include <bluetooth/mesh/models.h>
#include <zephyr/sys/util.h>
#include <zephyr/bluetooth/mesh/cdb.h>
#include "prov_helper.h"


#define MODEL_CLI_ID   0x5678

#define BT_MESH_MODEL_PROV_HELPER_CLI(_helper_cli)                                          \
            BT_MESH_MODEL_VND_CB(COMPANY_ID,                                        \
                     MODEL_CLI_ID,                                                      \
                     _bt_mesh_prov_helper_cli_opcode_list,                              							\
                     &(_helper_cli)->pub,                                               \
                     BT_MESH_MODEL_USER_DATA(struct bt_mesh_prov_helper_cli, _helper_cli),   \
                    &_bt_mesh_prov_helper_cli_cb)             

#define BT_MESH_PROV_HELPER_CLI_INIT()                                       \
	{                                                                      \
	    .pub = {.msg = NET_BUF_SIMPLE(BT_MESH_MODEL_BUF_LEN(           \
	    BT_MESH_PROV_HELPER_OP_NODEINFO,                      \
	    BT_MESH_PROV_HELPER_MSG_LEN_NODEINFO)) }              \
	}

struct bt_mesh_prov_helper_cli {
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
	//const struct bt_mesh_time_cli_handlers *handlers; 
};

int bt_mesh_prov_helper_cli_send_appkey(struct bt_mesh_model *model, uint8_t *app_key);
int bt_mesh_prov_helper_cli_send_netkey(struct bt_mesh_model *model, uint8_t *net_key);
int bt_mesh_prov_helper_cli_send_nodeinfo(struct bt_mesh_model *model, struct bt_mesh_cdb_node* node);

//int send_appkey(struct bt_mesh_model *model);
extern const struct bt_mesh_model_op _bt_mesh_prov_helper_cli_opcode_list[];
extern const struct bt_mesh_model_cb _bt_mesh_prov_helper_cli_cb;

#endif