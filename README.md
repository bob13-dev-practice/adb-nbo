# adb-nbo
실습을 통해 network byte order, host byte order에 대한 변환이 왜 필요한지 이해한다.
</br>
과제 출처: https://gitlab.com/gilgil/sns/-/wikis/byte-order/report-add-nbo
</br>
</br>
## Byte Order

컴퓨터는 데이터를 메모리에 저장할 때 바이트 단위로 나눠서 저장한다. 연속되는 바이트를 순서대로 저장해야 하는데, 이를 바이트 저장 순서(byte order)라 한다. 바이트가 저장되는 순서에 따라 `Big-Endian`과 `Little-Endian`의 두 가지 방식으로 나눌 수 있다.

💡 **엔디안(Endian)**: 컴퓨터 메모리와 같은 1차원 공간에서 여러 개의 연속된 대상을 배열하는 방법
</br></br>

### Big Endian (빅엔디안)

- 낮은 주소에 데이터의 높은 바이트부터 저장하는 방식이다.
- 평소 숫자를 사용하는 선형 방식과 같은 방식이며, 메모리에 저장된 순서 그대로 읽을 수 있어 이해하기가 쉽다.
- 네트워크 바이트와 호스트 바이트 오더 모두 **정방향**이다.
- ex) arm 일부
</br></br>

### Little Endian (리틀엔디안)

- 낮은 주소에 데이터의 낮은 바이트부터 저장하는 방식이다.
- 평소 숫자를 사용하는 선형 방식과는 반대로 읽어야 한다.
- 네트워크 바이트는 **정방향**, 호스트 바이트 오더는 **역방향**이다.
- ex) intel
</br></br>

### NBO(Network Byte Order)

Network Level에서 바이트를 표현하는 방법으로, 기본적으로 Big Endian 방식을 사용하고 있으며 Little Endian을 사용하는 시스템에서 네트워크 통신을 이용하려면 Big Endian으로 변환하는 과정을 거쳐야 한다.
</br></br>

### HBO(Host Byte Order)

Host Level에서 바이트를 표현하는 방법으로, CPU에 따라 Big Endian과 Little Endian으로 나뉜다.
</br></br>

### HBO에서 NBO로 변환하는 방법

C언어에서는 `#include <netinet/in.h>` 헤더를 이용하여 쉽게 변환할 수 있다.

해당 헤더를 이용한다면 컴파일러가 해당 CPU가 Little Endian인지, Big Endian인지 알아서 판단하고, 내부적으로 적절하게 동작할 수 있게 처리하는 메서드를 제공한다.

| Conversion | C Function | Size |
| --- | --- | --- |
| NBO to HBO | uint16_t ntohs(uint16_t netshort); | 2 Byte |
| HBO to NBO | uint16_t htons(uint16_t hostshort); | 2 Byte |
| NBO to HBO | uint32_t ntohl(uint32_t netlong); | 4 Byte |
| HBO to NBO | uint32_t htonl(uint32_t hostlong); | 4 Byte |

</br>

### 실습 1) 16bit ntohs 구현

네트워크 버퍼로 들어온 데이터가 시스템 내의 메모리에 저장될 때 리틀엔디안 방식에 의해 `0x3412`로 저장된다. 이를 빅엔디안으로 변환하는 과정이 필요하다.

```cpp
uint16_t my_ntohs(uint16_t n) {
		int n1 = n & 0xFF00; // 0x3400
		int n2 = n & 0x00FF; // 0x0012
		
		n1 >>= 8; // 0x0034
		n2 <<= 8; // 0x1200
		
		return n1 | n2; // 0x1234
}

void  write_0x1234() {
		uint8_t network_buffer[] = { 0x12, 0x34 };
		uint16_t* p = reinterpret_cast<uint16_t*>(network_buffer);
		uint16_t n = my_ntohs(*p); // TODO
		printf("16 bit number=0x%x\n", n);
}

int main() {
		write_0x1234();
}
```

```
16 bit number=0x1234
```
</br>

### 실습 2) 32bit nthol 구현

네트워크 버퍼로 들어온 데이터가 시스템 내의 메모리에 저장될 때 리틀엔디안 방식에 의해 `0x78563412`로 저장된다. 이를 빅엔디안으로 변환하는 과정이 필요하다.

```cpp
uint32_t my_ntohl(uint32_t n){
		int n1 = n & 0xFF000000; // 0x78000000
    int n2 = n & 0x00FF0000; // 0x00560000
		int n3 = n & 0x0000FF00; // 0x00003400
		int n4 = n & 0x000000FF; // 0x00000012

		n1 >>= 24; // 0x00000078
		n2 >>= 8; // 0x00005600
    n3 <<= 8; // 0x00340000
    n4 <<= 24; // 0x12000000
		
		return n1 | n2 | n3 | n4; // 0x12345678
}

void  write_0x12345678() {
	uint8_t network_buffer[] = { 0x12, 0x34, 0x56, 0x78 };
	uint32_t* p = reinterpret_cast<uint32_t*>(network_buffer);
	uint32_t n = my_ntohl(*p); // TODO
	printf("32 bit number=0x%x\n", n);
}

int main() {
    write_0x12345678();
}
```

```
32 bit number=0x12345678
```
</br>

### Reference
- https://www.tcpschool.com/c/c_refer_endian
- https://gitlab.com/gilgil/sns/-/wikis/byte-order/byte-order
- https://wonit.tistory.com/544
