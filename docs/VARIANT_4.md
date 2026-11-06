# Индивидуальное задание: вариант 4

## Тема

**Оптимизатор динамического ценообразования (Dynamic Pricing Core).**

Модуль рассчитывает адаптивную цену товара с учётом базовой цены, цены конкурента, складского остатка, всплеска спроса и минимальной маржи.

## Входные данные и ограничения

| Поле | Тип | Ограничение |
|---|---|---|
| `base_price` | `double` | `> 0.0` |
| `competitor_price` | `double` | `>= base_price * 0.2` |
| `current_stock` | `int` | `> 0`; нулевой остаток отклоняется |
| `demand_surge_factor` | `double` | от `0.5` до `5.0` |
| `min_allowable_margin` | `double` | от `0.0` до `90.0` процентов |

Валидация выполняется до расчёта. При ошибке API возвращает HTTP 400 и JSON с полем `error`.

## Алгоритм и формулы

1. Если остаток меньше 10 единиц, установить `scarcity_multiplier = 1.25`; иначе использовать `1.0`.
2. Рассчитать начальную цену:

```text
optimized_price = base_price * demand_surge_factor * scarcity_multiplier
```

3. Рассчитать нижнюю границу относительно конкурента:

```text
competitor_floor = competitor_price * 0.95
```

4. Если цена ниже `competitor_floor`, заменить её этим значением.
5. Рассчитать нижнюю границу маржи:

```text
margin_floor = base_price * (1.0 + min_allowable_margin / 100.0)
```

6. Если цена ниже `margin_floor`, заменить её этим значением.
7. Вернуть JSON с итоговым `optimized_price`.

## Контрольный пример

Запрос:

```json
{
  "base_price": 100.0,
  "competitor_price": 110.0,
  "current_stock": 5,
  "demand_surge_factor": 1.2,
  "min_allowable_margin": 10.0
}
```

Расчёт: `scarcity_multiplier = 1.25`, начальная цена `100 * 1.2 * 1.25 = 150`, границы конкурента и маржи равны `104.5` и `110`. Итоговая цена: `150`.

Ответ:

```json
{"optimized_price":150}
```

## REST API

Модуль подключён в `src/modules/individual/task.hpp` через `#include "variant_mine.hpp"`.

- `POST /api/v1.1/works/calc-individual` - расчёт, доступен авторизованному пользователю.
- `GET /api/v1.1/works/history-individual?limit=10` - история расчётов пользователя.

Успешный результат сохраняется в таблицу `works`.

## Запуск и проверка

```bash
sudo apt update
sudo apt install -y build-essential cmake sqlite3 libsqlite3-dev
cmake -S . -B build
cmake --build build -j2
./build/rest_server
curl http://localhost:8080/health
```

Получить токен:

```bash
curl -X POST http://localhost:8080/api/v1.1/auth/login \
  -H 'Content-Type: application/json' \
  -d '{"username":"user1","password":"user1"}'
```

Контрольный расчёт:

```bash
curl -X POST http://localhost:8080/api/v1.1/works/calc-individual \
  -H 'Authorization: Bearer <TOKEN>' \
  -H 'Content-Type: application/json' \
  -d '{"base_price":100,"competitor_price":110,"current_stock":5,"demand_surge_factor":1.2,"min_allowable_margin":10}'
```

Нулевой остаток должен вернуть HTTP 400.

## Тестирование

```bash
g++ -std=c++17 -Wall -Wextra -pedantic tests/test_variant4.cpp -I. -o /tmp/test_variant4
/tmp/test_variant4
```

Проверяются контрольный пример, границы `current_stock = 10`, `demand_surge_factor = 0.5`, `min_allowable_margin = 90`, а также ошибочные значения.
