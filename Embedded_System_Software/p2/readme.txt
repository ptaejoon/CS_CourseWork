1. app 프로그램
(1). app 디렉토리로 들어간다.
(2). make 입력
(3). adb push app (원하는 디렉토리)

2. module 프로그램
(1). module 디렉토리로 들어간다.
(2). make 입력
(3). adb push hw2.ko (원하는 디렉토리)

3. board에서 수행 작업
(1) hw2.ko 가 있는 디렉토리에서 insmod hw2.ko 입력
(2) mknod /dev/dev_driver c 242 0 입력
(3) app 가 있는 디렉토리에서 /app TIMER_INTERVAL[1-100] TIMER_CNT[1-100] TIMER_INIT[0010-8000]

4. 드라이버 unregister 시 board 수행 작업
(1) rmmod hw2 입력

driver name : dev_driver (등록시 /dev/dev_driver)
major number : 242