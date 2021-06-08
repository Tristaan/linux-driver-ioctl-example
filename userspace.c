#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>

int main() {
	int fd = open("/dev/customdevice", O_WRONLY);
	if (fd < 0) {
		printf("ERROR: %s", strerror(errno));
		return -1;
	}
	while (1) {
		int cmd = 0;
		printf("Choose command (1 or 2, other to exit)\n\r");
		scanf("%d", &cmd);

		if ( cmd == 1 || cmd == 2) {
			int ret = ioctl(fd, 0xFFAA + cmd, 8);
			if (ret < 0) {
				printf("ERROR: %s", strerror(errno));
				return -2;
			}
		} else {
			printf("Program exit");
			return 0;
		}
	}
	return 0;
}
