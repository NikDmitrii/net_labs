#Лабораторные по сетям

# Lab1

## Сборка C клиента

### Требования
- GCC компилятор
- Make

### Сборка
```bash
cd lab1/client
make
```

### Запуск
```bash
./client --net <ip> --port <port>
```

### Пример
```bash
# Запуск с broadcast адресом
./client --net 192.168.1.255 --port 12345
```

## Сборка C сервера

### Требования
- GCC компилятор
- Make

### Сборка
```bash
cd lab1/server
make
```

### Запуск
```bash
./server --net <ip> --port <port>
```

### Пример
```bash
# Запуск сервера
./server --net 192.168.1.100 --port 12345
```

## Сборка Java клиента

### Требования
- JDK 21 или выше
- Maven

### Сборка
```bash
cd lab1/java_client
mvn clean package
```

### Запуск
```bash
# Через java
java -jar target/java_client-1.0-SNAPSHOT-jar-with-dependencies.jar --net <ip> --port <port>

# Через maven
mvn exec:java -Dexec.args="--net <ip> --port <port>"
```

### Пример
```bash
# Запуск с broadcast адресом
java -jar target/java_client-1.0-SNAPSHOT-jar-with-dependencies.jar --net 192.168.1.255 --port 12345
```


## Сборка Java сервера

### Требования
- JDK 21 или выше
- Maven

### Сборка
```bash
cd lab1/java_server
mvn clean package
```

### Запуск
```bash
# Через java
java -jar target/java_server-1.0-SNAPSHOT-jar-with-dependencies.jar --net <ip> --port <port>

# Через maven
mvn exec:java -Dexec.args="--net <ip> --port <port>"
```

### Пример
```bash
# Запуск с broadcast адресом
java -jar target/java_server-1.0-SNAPSHOT-jar-with-dependencies.jar --net 192.168.1.255 --port 12345