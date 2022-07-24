---
marp: true
---

# iterator
## 반복자(iterator)의 종류
1. 입력 반복자(input iterator)
2. 출력 반복자(output iterator)
3. 순방향 반복자(forward iterator)
4. 양방향 반복자(bidirectional iterator)
5. 임의 접근 반복자(random access iterator)

---
### 입/출력 반복자(input/output iterator)
- 

### forward iterator
- 증가 연산자(++)를 사용하여 순방향으로만 이동할 수 있다.
- 참조 연산자( * )를 사용하여 해당 요소를 참조하거나 값을 변경할 수 있다.

### bidirectional iterator
- 감소 연산자(--)도 사용할 수 있다.

### random access iterator
- 첨자 연산자([])나 덧셈 대입(+=), 뺄셈 대입(-=)을 사용할 수 있다.
- 증감을 통해 양방향으로 이동할 수 있고, 요소의 순서를 판별하기 위한 관계 연산자도 사용 가능하다.
- 일반 포인터의 기능을 대부분 수행할 수 있다.
- std::vector, std::array 등에서 제공하는 이터레이터.

### Refs
- [iterator의 종류](http://www.tcpschool.com/cpp/cpp_iterator_category)
- [iterator, traits](http://egloos.zum.com/sweeper/v/3007176)
---
