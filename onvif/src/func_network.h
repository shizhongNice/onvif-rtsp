/**
 * func_network.h
 **/

#ifndef __FUNC_NETWORK_H__

#ifdef __cplusplus
extern "C" {
#endif

int device_ip_get(char *interface, char *addr);
int device_mac_get(char *interface, unsigned char *pMac);
int device_mac_get_by_str(char *interface, unsigned char *pMac);
#ifdef __cplusplus
}
#endif

#endif /* __FUNC_NETWORK_H__ */
