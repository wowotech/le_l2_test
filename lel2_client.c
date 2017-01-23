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

#include <sys/time.h>

#include "common.h"

static char		test_data[TEST_BUF_LEN];

static char		test_buf[TEST_BUF_LEN];

int main(int argc , char **argv) {
	int sockfd;
	int i, err = 0, test_counter = 0;
	ssize_t ret;

	struct sockaddr_l2 bind_addr = { 0 };
	struct sockaddr_l2 conn_addr = { 0 };

	struct timeval starttime, endtime;

	float time_consumed_us;

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

	printf("send, receive and verify...\n");

	/* prepare for test data */
	for (i = 0; i < TEST_BUF_LEN; i++)
		test_data[i] = random() & 0xff;

	time_consumed_us = 0;
	while (++test_counter <= TEST_CNT) {
		printf("test_counter: %d\n", test_counter);

		gettimeofday(&starttime, NULL);

		/* write */
		ret = write(sockfd, test_data, TEST_BUF_LEN);
		if (ret != TEST_BUF_LEN) {
			perror("write error\n");
			return -1;
		}

		/* read back */
		ret = read(sockfd, test_buf, TEST_BUF_LEN);

		gettimeofday(&endtime, NULL);

		/* verify */
		if (ret != TEST_BUF_LEN) {
			perror("length verify failed\n");
			return -1;
		}
		if (memcmp(test_buf, test_data, TEST_BUF_LEN) != 0) {
			perror("content verify failed\n");
			return -1;
		}

		time_consumed_us += (endtime.tv_usec - starttime.tv_usec
				     + 1000000 * (endtime.tv_sec
						  - starttime.tv_sec));
	}

	printf("TEST END: time consumed %f us, bps is %d\n", time_consumed_us,
	       TEST_BUF_LEN * TEST_CNT * 1000000 / (int)time_consumed_us);
	return 0;
}
