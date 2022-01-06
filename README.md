### 제작목적

---

DirectX를 활용하여 게임 제작에 필요한 툴을 제작하여 실제 게임개발에 사용할 수 되는 기능들에 대해 이해하고 게임엔진구조를 만들어보는것

### 제작기간

**2021.09.09 ~ 2021.12.27**

### 사용기술

- C++ 
- imgui
- DirectX
- Box2D Engine
---

### 제작한 기능

- ECS
    - Transform
    - LIGHT2D
    - RIGIDBODY2D
    - COLLIDER2D
    - MeshRender
    - Particle System
    - Animator2D, Animation2D
    - TileMap
    - UI
    - Script  
- Tool
    - Hierachy View
    - Inspector View
    - Resource View
    - Animation Tool
    - TileMap Tool
    - ToolView & GamePlay View

### 프로젝트를 진행하면서 느낀점

---

- 이전까지는 상용엔진들의 기능을 그저 쓰기만 했는데 프로젝트이후 게임엔진이 어떻게 동작을 하는지, 게임엔진에서 각 컴포넌트들이 어떤역할을 하는지 그 이유와 원리를 파악할 수 있었고, 이를 기반으로 더 다양하게 기능들을 활용할 수 있다는것을 알았다.

- 이전의 프로젝트의 경험으로 좋은 툴을 만들기 위해 집중을 했으며, 실제 사용해 본결과 만족할만한 툴을 만들기가 매우 어렵고 시간이 오래 소모된다는 사실을 깨달았다. 또한 사용자가 쓰기 편하도록 개발해야한다는 점이 매우 어려웠고, 상용에 나와있는 툴들을 많이 참고하면서 설계하나하나에 모두 이유가 있다는것을 알 수 있었다.

- 메모리 누수가 정말 사소한 코드 한줄에 의해 발생한다는 사실을 알았으며, 리팩토링 과정을 왜 해야하는지 다시한번 느낄 수 있었다. 특히 이전에는 Garbege Collector가 있는 언어들만 사용했기에 동적할당을 무분별하게 사용하는 경향이 있었는데 이번 프로젝트로 이러한 습관이 엄청나게 메모리를 잡아먹게된다는 사실을 알게되어 좀더 클린한 코드를 만들수 있도록 노력하게 되었다.

### 게임엔진 영상

---
- 게임 툴 사용 영상은 아래 링크에서 볼 수 있습니다.
    - [엔진 영상](https://youtu.be/irAoCaeDUL0)
    - [타일맵 툴 영상](https://youtu.be/s0NGX6nVL8k)