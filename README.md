<div align="center">
  <h1>IDE для ассемблера NASM</h1>
  <img width="1282" height="675" alt="intro" src="https://github.com/user-attachments/assets/e0a1ff6c-9240-4cdd-9306-43edc08c8662" />
</div>

### О программе
Интегрированная среда разработки, которая объединяет ассемблер NASM и компилятор GCC. Она автоматизирует процессы вызова, перехватывает их выводы и представляет результаты в удобном, интуитивно понятном формате.

> Этот проект является моей дипломной работой в бакалавриате.

### Установка зависимостей
C помощью **vcpkg**:
```bash
integrate install
vcpkg install robin-hood-hashing nativefiledialog-extended glew glm glfw3 stb nlohmann-json soil imgui[opengl3-binding,glfw-binding] --triplet x64-windows-static
```

### Сборка
Если зависимости установлены:
* Просто запустить проект в Visual studio и нажать собрать.
