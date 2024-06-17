
#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/mesh.h>
#include "prov_helper_cli.h"
#include "msg.h"

#define LOG_LEVEL LOG_LEVEL_DBG
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(prov_helper_cli);


static int bt_mesh_prov_helper_cli_init(struct bt_mesh_model *model){

    struct bt_mesh_prov_helper_cli *helper_cli = model->user_data;

    helper_cli->model = model;
    net_buf_simple_init(helper_cli->pub.msg, 0);
    bt_mesh_msg_ack_ctx_init(&helper_cli->ack_ctx);

    LOG_DBG("Provisioner Helper Client initialized");

    return 0;
}

static void bt_mesh_prov_helper_cli_reset(struct bt_mesh_model *model){

    struct bt_mesh_prov_helper_cli *helper_cli = model->user_data;

    net_buf_simple_reset(helper_cli->pub.msg);
    bt_mesh_msg_ack_ctx_reset(&helper_cli->ack_ctx);

    LOG_DBG("Provisioner Helper Client Reset");

}

const struct bt_mesh_model_cb _bt_mesh_prov_helper_cli_cb = {
    .init = bt_mesh_prov_helper_cli_init,
    .reset = bt_mesh_prov_helper_cli_reset,
};

static int send_appkey(struct bt_mesh_model *model, uint8_t *app_key, uint16_t addr){

    struct bt_mesh_prov_helper_cli *helper_cli = model->user_data;

    struct bt_mesh_msg_ctx ctx = BT_MESH_MSG_CTX_INIT_APP(0, addr);
    

    bt_mesh_model_msg_init(model->pub->msg, BT_MESH_PROV_HELPER_OP_APPKEY);
    net_buf_simple_add_mem(model->pub->msg, app_key, BT_MESH_PROV_HELPER_MSG_LEN_APPKEY);

    const struct bt_mesh_msg_rsp_ctx rsp = {
		.ack = &helper_cli->ack_ctx,
		.op = BT_MESH_PROV_HELPER_OP_APPKEY,
		.user_data = NULL,
		.timeout = 10000,
	};

    // Fill message buffer

    //return bt_mesh_model_publish(model);
    return bt_mesh_msg_ackd_send(model, &ctx, model->pub->msg, &rsp);
}

int bt_mesh_prov_helper_cli_send_appkey(struct bt_mesh_model *model, uint8_t *app_key, uint16_t addr){

    return send_appkey(model, app_key, addr);

}

static int send_netkey(struct bt_mesh_model *model, uint8_t *net_key, uint16_t addr){

    struct bt_mesh_prov_helper_cli *helper_cli = model->user_data;

    struct bt_mesh_msg_ctx ctx = BT_MESH_MSG_CTX_INIT_APP(0, addr);
    

    bt_mesh_model_msg_init(model->pub->msg, BT_MESH_PROV_HELPER_OP_NETKEY);
    net_buf_simple_add_mem(model->pub->msg, net_key, BT_MESH_PROV_HELPER_MSG_LEN_NETKEY);

    const struct bt_mesh_msg_rsp_ctx rsp = {
		.ack = &helper_cli->ack_ctx,
		.op = BT_MESH_PROV_HELPER_OP_NETKEY,
		.user_data = NULL,
		.timeout = 10000,
	};

    // Fill message buffer

    //return bt_mesh_model_publish(model);
    return bt_mesh_msg_ackd_send(model, &ctx, model->pub->msg, &rsp);

}

int bt_mesh_prov_helper_cli_send_netkey(struct bt_mesh_model *model, uint8_t *net_key, uint16_t addr){
    return send_netkey(model, net_key, addr);
}

static int send_addrinfo(struct bt_mesh_model *model, uint16_t start, uint16_t end,
                         uint16_t origin, uint16_t destination_addr, uint8_t devices_to_provision,
                         uint8_t provisioning_time){

    struct bt_mesh_prov_helper_cli *helper_cli = model->user_data;

    struct bt_mesh_msg_ctx ctx = BT_MESH_MSG_CTX_INIT_APP(0, destination_addr);

    bt_mesh_model_msg_init(model->pub->msg, BT_MESH_PROV_HELPER_OP_ADDRINFO);

    net_buf_simple_add_le16(model->pub->msg, start);
    net_buf_simple_add_le16(model->pub->msg, end);
    net_buf_simple_add_le16(model->pub->msg, origin);
    net_buf_simple_add_u8(model->pub->msg, devices_to_provision);
    net_buf_simple_add_u8(model->pub->msg, provisioning_time);

    const struct bt_mesh_msg_rsp_ctx rsp = {
		.ack = &helper_cli->ack_ctx,
		.op = BT_MESH_PROV_HELPER_OP_ADDRINFO,
		.user_data = NULL,
		.timeout = 10000,
	};

    //return bt_mesh_model_publish(model);
    return bt_mesh_msg_ackd_send(model, &ctx, model->pub->msg, &rsp);
}

int bt_mesh_prov_helper_cli_send_addrinfo(struct bt_mesh_model *model,uint16_t start, uint16_t end,
                                          uint16_t origin, uint16_t destination_addr, uint8_t devices_to_provision,
                                          uint8_t provisioning_time){

    return send_addrinfo(model, start, end, origin, destination_addr, devices_to_provision, provisioning_time);

}

static int send_nodeinfo(struct bt_mesh_model *model, struct bt_mesh_cdb_node* node, uint16_t addr){

    struct bt_mesh_prov_helper_cli *helper_cli = model->user_data;

    struct bt_mesh_msg_ctx ctx = BT_MESH_MSG_CTX_INIT_APP(0, addr);

    bt_mesh_model_msg_init(model->pub->msg, BT_MESH_PROV_HELPER_OP_NODEINFO);

    net_buf_simple_add_mem(model->pub->msg, node->uuid, 16);
    net_buf_simple_add_le16(model->pub->msg, node->addr);
    net_buf_simple_add_le16(model->pub->msg, node->net_idx);
    net_buf_simple_add_u8(model->pub->msg, node->num_elem);
    net_buf_simple_add_mem(model->pub->msg, node->dev_key.key, 16);
    net_buf_simple_add_mem(model->pub->msg, node->flags, sizeof(node->flags));

    const struct bt_mesh_msg_rsp_ctx rsp = {
		.ack = &helper_cli->ack_ctx,
		.op = BT_MESH_PROV_HELPER_OP_NODEINFO,
		.user_data = NULL,
		.timeout = 10000,
	};

    //return bt_mesh_model_publish(model);
    return bt_mesh_msg_ackd_send(model, &ctx, model->pub->msg, &rsp);

}

int bt_mesh_prov_helper_cli_send_nodeinfo(struct bt_mesh_model *model, struct bt_mesh_cdb_node* node, uint16_t addr){

    return send_nodeinfo(model, node, addr);

}

const struct bt_mesh_model_op _bt_mesh_prov_helper_cli_opcode_list[] = {
    { 
        BT_MESH_PROV_HELPER_OP_APPKEY,
        BT_MESH_LEN_EXACT(BT_MESH_PROV_HELPER_MSG_LEN_APPKEY),
        NULL
    },
    { 
        BT_MESH_PROV_HELPER_OP_NETKEY,
        BT_MESH_LEN_EXACT(BT_MESH_PROV_HELPER_MSG_LEN_NETKEY),
        NULL
    },
    { 
        BT_MESH_PROV_HELPER_OP_NODEINFO,
        BT_MESH_LEN_EXACT(BT_MESH_PROV_HELPER_MSG_LEN_NODEINFO),
        NULL
    },
    { 
        BT_MESH_PROV_HELPER_OP_ADDRINFO,
        BT_MESH_LEN_EXACT(BT_MESH_PROV_HELPER_MSG_LEN_ADDRINFO),
        NULL
    },
    { 
        BT_MESH_PROV_HELPER_OP_APPKEY_STATUS,
        BT_MESH_LEN_EXACT(BT_MESH_PROV_HELPER_MSG_LEN_APPKEY_STATUS),
        NULL
    },
    { 
        BT_MESH_PROV_HELPER_OP_NETKEY_STATUS,
        BT_MESH_LEN_EXACT(BT_MESH_PROV_HELPER_MSG_LEN_NETKEY_STATUS),
        NULL
    },
    { 
        BT_MESH_PROV_HELPER_OP_NODEINFO_STATUS,
        BT_MESH_LEN_EXACT(BT_MESH_PROV_HELPER_MSG_LEN_NODEINFO_STATUS),
        NULL
    },
    { 
        BT_MESH_PROV_HELPER_OP_ADDRINFO_STATUS,
        BT_MESH_LEN_EXACT(BT_MESH_PROV_HELPER_MSG_LEN_ADDRINFO_STATUS),
        NULL
    },
    BT_MESH_MODEL_OP_END,
};
