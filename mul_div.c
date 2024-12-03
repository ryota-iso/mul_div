#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

#define	LOGIN	"ryota-iso"

long long count;
int loop;

#define	LOOP_SEC	(2)

static __inline int mul(int x, int y) {
	unsigned int partials[16];

	partials[0] = (y & -(x & 1));
	partials[1] = ((y << 1) & -(x & (1 << 1)));
	partials[2] = ((y << 2) & -(x & (1 << 2)));
	partials[3] = ((y << 3) & -(x & (1 << 3)));

	partials[4] = ((y << 4) & -(x & (1 << 4)));
	partials[5] = ((y << 5) & -(x & (1 << 5)));
	partials[6] = ((y << 6) & -(x & (1 << 6)));
	partials[7] = ((y << 7) & -(x & (1 << 7)));

	partials[8] = ((y << 8) & -(x & (1 << 8)));
	partials[9] = ((y << 9) & -(x & (1 << 9)));
	partials[10] = ((y << 10) & -(x & (1 << 10)));
	partials[11] = ((y << 11) & -(x & (1 << 11)));

	partials[12] = ((y << 12) & -(x & (1 << 12)));
	partials[13] = ((y << 13) & -(x & (1 << 13)));
	partials[14] = ((y << 14) & -(x & (1 << 14)));
	partials[15] = ((y << 15) & -(x & (1 << 15)));

	return partials[0] + partials[1] + partials[2] + partials[3] + partials[4] + partials[5] + partials[6] + partials[7] + partials[8] + partials[9] + partials[10] + partials[11] + partials[12] + partials[13] + partials[14] + partials[15];
}

// static __inline int div(int x,int y) {
// //
// // 以下を削除してここに除算のロジックを入れてください。x=32bit , y=32bit, result=32bit
// //
// 	return x/y;
// }

void alarm_mul(int signum){
	printf("MUL: %lld K instructions/sec\n", count/LOOP_SEC/1000);
	loop = 0;
}

void alarm_div(int signum){
	printf("DIV: %lld K instructions/sec\n", count/LOOP_SEC/1000);
	loop = 0;
}

int main() {
	int x,y;


	printf("Start (%s program)\n", LOGIN);


	// Benchmark MUL
	count = 0;
	loop = 1;
	signal(SIGALRM, alarm_mul);
	alarm(LOOP_SEC);
	for (x=1; loop ; ++x) {
		for (y=1; y<0xffff; ++y) {
			if (mul(x,y) != x*y) {
				printf("MUL: Calculation results are incorrect. results=%d, expected=%d\n", mul(x,y), x*y);
				return(-1);
			}
			++count;
		}
	}

	// Benchmark DIV
	// count = 0;
	// loop = 1;
	// signal(SIGALRM, alarm_div);
	// alarm(LOOP_SEC);
	// for (x=0x0fffffff; loop ; --x) {
	// 	for (y=1; y<0xffff; ++y) {
	// 		if (div(x,y) != x/y) {
	// 			printf("DIV: Calculation results are incorrect. results=%d, expected=%d\n", div(x,y), x/y);
	// 			return(-1);
	// 		}
	// 		++count;
	// 	}
	// }

	return 0;
}
