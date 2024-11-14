// MUL: 13198 K instructions/sec
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

#define	LOGIN	"ryota-iso"

long long count;
int loop;

#define	LOOP_SEC	(2)

static __inline int mul(int x,int y) {
  int result = 0;
  int negative = 0;

  // 符号の処理（絶対値で計算し、最後に符号を調整する）
  if (x < 0) {
    x = -x;
    negative = !negative;
  }
  if (y < 0) {
    y = -y;
    negative = !negative;
  }

  while (y > 0) {
    // 最下位ビットが1の場合、加算
    if (y & 1) {
      result += x;
    }
    // 倍数を計算するため、左シフト
    x <<= 1;
    // 商を進めるため、右シフト
    y >>= 1;
  }

  // 符号の調整
  return negative ? -result : result;
}

static __inline int div(int x,int y) {
//
// 以下を削除してここに除算のロジックを入れてください。x=32bit , y=32bit, result=32bit
//
	return x/y;
}

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
	count = 0;
	loop = 1;
	signal(SIGALRM, alarm_div);
	alarm(LOOP_SEC);
	for (x=0x0fffffff; loop ; --x) {
		for (y=1; y<0xffff; ++y) {
			if (div(x,y) != x/y) {
				printf("DIV: Calculation results are incorrect. results=%d, expected=%d\n", div(x,y), x/y);
				return(-1);
			}
			++count;
		}
	}

	return 0;

}
