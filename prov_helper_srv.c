#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/mesh.h>

#include "prov_helper_srv.h"

#define LOG_LEVEL LOG_LEVEL_DBG
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(prov_helper_srv);


static int bt_mesh_prov_helper_srv_init(struct bt_mesh_model *model){

    struct bt_mesh_prov_helper_srv *helper_srv = model->user_data;

    helper_srv->model = model;
    net_buf_simple_init(helper_srv->pub.msg, 0);
    bt_mesh_msg_ack_ctx_init(&helper_srv->ack_ctx);

    LOG_DBG("Provisioner Helper Server initialized");

    return 0;
}

static void bt_mesh_prov_helper_srv_reset(struct bt_mesh_model *model){

    struct bt_mesh_prov_helper_srv *helper_srv = model->user_data;

    net_buf_simple_reset(helper_srv->pub.msg);
    bt_mesh_msg_ack_ctx_reset(&helper_srv->ack_ctx);

    LOG_DBG("Provisioner Helper Server Reset");

}

static int handle_message_appkey(struct bt_mesh_model *model,
                                  struct bt_mesh_msg_ctx *ctx,
                                  struct net_buf_simple *buf){
    
    struct bt_mesh_prov_helper_srv *srv = model->user_data;
    LOG_INF("Received appkey message");

    if(srv->handlers && srv->handlers->prov_helper_message_appkey){
        LOG_INF("Calling app key handler");
        srv->handlers->prov_helper_message_appkey(srv, ctx, buf);
    }

    return 0;
}

static int handle_message_netkey(struct bt_mesh_model *model,
                                  struct bt_mesh_msg_ctx *ctx,
                                  struct net_buf_simple *buf){
    struct bt_mesh_prov_helper_srv *srv = model->user_data;

    LOG_INF("Received netkey message");
    
    if(srv->handlers && srv->handlers->prov_helper_message_netkey){
        LOG_INF("Calling net key handler");
        srv->handlers->prov_helper_message_netkey(srv, ctx, buf);
    }
    
    return 0;
}

static int handle_message_nodeinfo(struct bt_mesh_model *model,
                                  struct bt_mesh_msg_ctx *ctx,
                                  struct net_buf_simple *buf){
    struct bt_mesh_prov_helper_srv *srv = model->user_data;
    
    //LOG_INF("Received nodeinfo message");
    if(srv->handlers && srv->handlers->prov_helper_message_nodeinfo){
        //LOG_INF("Calling node info handler");
        srv->handlers->prov_helper_message_nodeinfo(srv, ctx, buf);
    }
    return 0;
}

static int handle_message_addrinfo(struct bt_mesh_model *model,
                                  struct bt_mesh_msg_ctx *ctx,
                                  struct net_buf_simple *buf){
    
    struct bt_mesh_prov_helper_srv *srv = model->user_data;
    
    LOG_INF("Received addrinfo message");
    if(srv->handlers && srv->handlers->prov_helper_message_addrinfo){
        LOG_INF("Calling addr info handler");
        srv->handlers->prov_helper_message_addrinfo(srv, ctx, buf);
    }
    return 0;
}

const struct bt_mesh_model_cb _bt_mesh_prov_helper_srv_cb = {
    .init = bt_mesh_prov_helper_srv_init,
    .reset = bt_mesh_prov_helper_srv_reset,
};

const struct bt_mesh_model_op _bt_mesh_prov_helper_srv_opcode_list[] = {
    { 
        BT_MESH_PROV_HELPER_OP_APPKEY,
        BT_MESH_LEN_EXACT(BT_MESH_PROV_HELPER_MSG_LEN_APPKEY),
        handle_message_appkey
    },
    { 
        BT_MESH_PROV_HELPER_OP_NETKEY,
        BT_MESH_LEN_EXACT(BT_MESH_PROV_HELPER_MSG_LEN_NETKEY),
        handle_message_netkey
    },
    { 
        BT_MESH_PROV_HELPER_OP_NODEINFO,
        BT_MESH_LEN_EXACT(BT_MESH_PROV_HELPER_MSG_LEN_NODEINFO),
        handle_message_nodeinfo
    },
    { 
        BT_MESH_PROV_HELPER_OP_ADDRINFO,
        BT_MESH_LEN_EXACT(BT_MESH_PROV_HELPER_MSG_LEN_ADDRINFO),
        handle_message_addrinfo
    },
    BT_MESH_MODEL_OP_END,
};
