#ifndef BT_MESH_PROV_HELPER_H__
#define BT_MESH_PROV_HELPER_H__

#include <bluetooth/mesh/model_types.h>
#include <bluetooth/mesh/models.h>
#include <zephyr/sys/util.h>
#include <zephyr/bluetooth/mesh/cdb.h>

#define COMPANY_ID 0x03D3

#define BT_MESH_PROV_HELPER_OP_APPKEY			BT_MESH_MODEL_OP_3(0x01, COMPANY_ID)
#define BT_MESH_PROV_HELPER_OP_NETKEY			BT_MESH_MODEL_OP_3(0x02, COMPANY_ID)
#define BT_MESH_PROV_HELPER_OP_NODEINFO			BT_MESH_MODEL_OP_3(0x03, COMPANY_ID)
#define BT_MESH_PROV_HELPER_OP_ADDRINFO			BT_MESH_MODEL_OP_3(0x04, COMPANY_ID)

#define BT_MESH_PROV_HELPER_MSG_LEN_APPKEY          16
#define BT_MESH_PROV_HELPER_MSG_LEN_NETKEY          16
#define BT_MESH_PROV_HELPER_MSG_LEN_NODEINFO        (sizeof(struct bt_mesh_cdb_node) - 3)
#define BT_MESH_PROV_HELPER_MSG_LEN_ADDRINFO        (3 * sizeof(uint16_t) + 2 * sizeof(uint8_t))



#endif