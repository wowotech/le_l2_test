/*
 * Name:    lel2_server.c
 *
 * Purpose: the server side test code.
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

#include "common.h"

static char		test_buf[TEST_BUF_LEN];

int main(void)
{
	int sockfd, newfd;
	int err = 0;
	ssize_t ret;

	socklen_t sin_size;

	struct sockaddr_l2 my_addr = { 0 };
	struct sockaddr_l2 peer_addr = { 0 };

	printf("socket\n");
	sockfd = socket(PF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);
	if (sockfd < 0) {
		printf(" no socket\n");
		return -1;
	}

	printf("bind\n");
	my_addr.l2_family = AF_BLUETOOTH;
	my_addr.l2_cid = htobs(TEST_CID);
	bacpy(&my_addr.l2_bdaddr, BDADDR_ANY);
	my_addr.l2_bdaddr_type = BDADDR_LE_PUBLIC;
	err = bind(sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr));
	if (err) {
		perror("L2CAP bind failed\n");
		return -1;
	}

	printf("listen\n");
	if (listen(sockfd, 10) == -1) {
		perror("listen");
		exit(1);
	}

	printf("accept\n");
	sin_size = sizeof(struct sockaddr_in);
	if ((newfd = accept(sockfd, (struct sockaddr *)&peer_addr, &sin_size))==-1) {
		perror("accept");
		exit(1);
	}

	printf("receive and send back...\n");

	while (1) {
		ret = read(newfd, test_buf, TEST_BUF_LEN);
		if (ret != TEST_BUF_LEN) {
			perror("read error");
			exit(1);
		}
		printf("received\n");

		ret = write(newfd, test_buf, TEST_BUF_LEN);
		if (ret != TEST_BUF_LEN) {
			perror("write error\n");
			return -1;
		}
	}

	return 0;
}
