#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "func_network.h"

#include "xcam_log.h"
#define LOG_TAG "ONVIF"

#ifdef __cplusplus
extern "C" {
#endif

unsigned char HOST_MAC[6] = {0};

int device_ip_get(char *interface, char *addr)
{
	int ret;
    int inet_sock;
    struct ifreq ifr;

    inet_sock = socket(AF_INET, SOCK_DGRAM, 0);
    strcpy(ifr.ifr_name, "eth0");
	ret = ioctl(inet_sock, SIOCGIFADDR, &ifr);
	if (ret < 0)
		return ret;

	strcpy(addr, inet_ntoa(((struct sockaddr_in *)&(ifr.ifr_addr))->sin_addr));

	return 0;
}

int device_mac_get(char *interface, unsigned char *pMac)
{
	struct ifreq ifreq;
	int sockfd = 0;
	unsigned char mac[6] = {0};

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return -1;

	strcpy(ifreq.ifr_name, interface);

	if(ioctl(sockfd, SIOCGIFHWADDR, &ifreq) < 0)
	{
		close(sockfd);
		return -2;
	}

	memcpy(mac, ifreq.ifr_hwaddr.sa_data, 6);
	LOG_DBG(LOG_TAG, "MAC:%02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

	if(pMac != NULL)
	{
		memcpy(pMac, mac, 6);
	}

	close(sockfd);

	return 0;
}

    int device_mac_get_by_str(char *interface, unsigned char *pMac)
{
	struct ifreq ifreq;
	int sockfd = 0;
	unsigned char mac[6] = {0};

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return -1;

	strcpy(ifreq.ifr_name, interface);

	if(ioctl(sockfd, SIOCGIFHWADDR, &ifreq) < 0)
	{
		close(sockfd);
		return -2;
	}

	memcpy(mac, ifreq.ifr_hwaddr.sa_data, 6);
	LOG_DBG(LOG_TAG, "MAC:%02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

	sprintf((char*)pMac, "%02X-%02X-%02X-%02X-%02X-%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

	close(sockfd);

	return 0;
}

#ifdef __cplusplus
}
#endif
