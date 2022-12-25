# Лабораторная работа №4 Менеджер памяти

## Результат

Реализован класс MemoryManager, который выделяет память по стратегии Fixed Size Allocator для запросов 16, 32, 64, 128, 256, 512 байт. Coalesce Allocator для размеров от 512 байт до 10 мегабайт, и совершает прямые запросы к операционной системе для размеров более 10 мегабайт.

- [MemoryManager](./MemoryManager.hpp)
- [FixedSizeAllocator](./FixedAllocator.hpp)
- [CoalesceAllocator](./CoalesceAllocator.hpp)
- [OSAllocator](./HugeAllocator.hpp)

[Пример использования](./main.cpp)

## Пример вывода методов `DumpStat()` и `DumpBlocks()`

### Fixed Size Allocator

```
Statistics:
        FSA:
--------------------
Block size: 16 bytes
Page count: 1
Free block count: 3
Occupied block count: 3
Page size: 128 bytes

Dump:
        FSA:
--------------------
Block size: 16 bytes
Page number: 0
Occupied block: 000001C08A080030 Size: 16 bytes
Occupied block: 000001C08A080050 Size: 16 bytes
Occupied block: 000001C08A080070 Size: 16 bytes
Free block: 000001C08A080090 Size: 16 bytes
Free block: 000001C08A0800B0 Size: 16 bytes
Free block: 000001C08A0800D0 Size: 16 bytes
```

### Coalesce Allocator

```
Statistics:
        Coalesce Allocator:
--------------------
Page count: 1
Free block count: 1
Occupied block count: 3
Page size: 12582912 bytes

Dump:
        Coalesce Allocator:
--------------------
Page number: 0
Occupied block: 000001A646350050 size: 1024 bytes
Occupied block: 000001A646350488 size: 2048 bytes
Occupied block: 000001A646350CC0 size: 4096 bytes
Empty block: 000001A646351CF8 size: 12575664 bytes
```

### OS Allocator

```
Statistics:
        OS Allocator:
--------------------
Page count: 3
Allocation size: 112640 bytes

Dump:
        OS Allocator:
--------------------
Occupied block: 0000017B17F10018 size: 12288bytes
Occupied block: 0000017B17F20018 size: 30720bytes
Occupied block: 0000017B197D0018 size: 69632bytes
```
