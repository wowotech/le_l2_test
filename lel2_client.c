/*
 * Name:    lel2_client.c
 *
 * Purpose: the client side test code.
 *
 * Copyright (C) 2017 wowotech
 * Subject to the GNU Public License, version 2.
 *
 * Created By:         wowo<www.wowotech.net>
 * Created Date:       2017-01-20
 *
 * ChangeList:
 * Created in 2017-01-20 by wowo;
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#define TEST_CID		4	/* TEST CID, 4 is ATT */

static char test_data[] = "Test data for le_l2_test. 12345abc!#";

int main(int argc , char **argv) {
	int sockfd;
	int err = 0;
	ssize_t ret;

	struct sockaddr_l2 bind_addr = { 0 };
	struct sockaddr_l2 conn_addr = { 0 };

	printf("socket\n");

	sockfd = socket(PF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);
	if (sockfd < 0) {
		printf("no socket\n");
		return -1;
	}

	printf("bind\n");

	bind_addr.l2_family = AF_BLUETOOTH;
	bind_addr.l2_cid = htobs(TEST_CID);
	bacpy(&bind_addr.l2_bdaddr, BDADDR_ANY);
	bind_addr.l2_bdaddr_type = BDADDR_LE_PUBLIC;

	err = bind(sockfd, (struct sockaddr*)&bind_addr, sizeof(bind_addr));
	if (err) {
		perror("L2CAP bind failed\n");
		return -1;
	}

	printf("connect\n");

	conn_addr.l2_family = AF_BLUETOOTH;
	conn_addr.l2_cid = htobs(TEST_CID);
	str2ba(argv[1], &conn_addr.l2_bdaddr);
	conn_addr.l2_bdaddr_type = BDADDR_LE_PUBLIC;

	err = connect(sockfd, (struct sockaddr*)&conn_addr, sizeof(conn_addr));
	if (err) {
		perror("L2CAP connect failed\n");
		return -1;
	}

	printf("send...\n");

	while (1) {
		ret = write(sockfd, test_data, sizeof(test_data));
		if (ret != sizeof(test_data)) {
			perror("write\n");
		}
		usleep(1000000);
	}

	printf("exit\n");
	return 0;
}
