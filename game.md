> **capacity (infrastruktura)** vs **demand (ruch)**

Czyli przestajesz mieć klasyczny idle, a zaczynasz mieć **system równoważenia** — to znacząco podnosi głębię bez rozwalania UI. Poniżej porządkuję to w model, który możesz od razu zaimplementować.

---

# 🧠 Nowy core loop

1. Kupujesz / ulepszasz serwery → zwiększasz **capacity**
2. Kupujesz kampanie → zwiększasz **traffic**
3. Dochód zależy od:

   * tego ile ruchu jesteś w stanie obsłużyć

👉 klucz:

> **income = min(traffic, capacity)**

---

# 🧩 Główne systemy

## 🖥️ 1. Capacity (serwery)

Każdy serwer daje:

```text
capacity_per_server = base_capacity * level
```

Przykład:

* lvl 1 → 10 req/s
* lvl 5 → 50 req/s

👉 upgrade = więcej capacity

---

## 📢 2. Traffic (kampanie)

Zamiast kupować „dochód”, kupujesz **źródło ruchu**:

### Typy (proste, pod Arduboy):

* Campaign 1 → +10 traffic
* Campaign 2 → +25 traffic
* Campaign 3 → +60 traffic

Każda kampania:

* kosztuje
* daje stały traffic

---

## 💰 3. Dochód

```text
income_per_tick = min(total_capacity, total_traffic) * value_per_unit
```

👉 jeśli:

* masz dużo serwerów → ale mało ruchu → tracisz potencjał
* masz dużo ruchu → ale mało serwerów → masz bottleneck

---

# 🔥 Nowa dynamika gry

To co się pojawia:

### 🟢 idealny stan:

```
capacity ≈ traffic
```

### 🔴 za dużo capacity:

* zmarnowane pieniądze

### 🔴 za dużo traffic:

* „przegrzewanie” (utrata potencjalnego zysku)

👉 gracz cały czas balansuje

---

# 📊 Przykładowa tabela (early game)

## 🖥️ Serwery

| Level | Capacity | Upgrade Cost |
| ----- | -------- | ------------ |
| 1     | 10       | 10           |
| 2     | 20       | 15           |
| 3     | 30       | 23           |
| 4     | 40       | 34           |
| 5     | 50       | 51           |

---

## 📢 Kampanie

| Campaign | Traffic | Cost |
| -------- | ------- | ---- |
| Small    | 10      | 50   |
| Medium   | 25      | 120  |
| Large    | 60      | 300  |

---

# 🎯 Przykład działania

Gracz ma:

* capacity = 40
* traffic = 25

👉 income = 25

---

Kupuje kampanię:

* traffic = 50

👉 income = 40 (limit capacity)

---

Upgrade serwera:

* capacity = 60

👉 income = 50

---

💡 To daje bardzo czytelny feedback — idealne na Arduboy

---

# 🖥️ UI (dopasowane do zmiany)

Nowy layout:

```text
+----------------------+
| $1234   +40/s        |
|----------------------|
| CAP: 60              |
| TRA: 50              |
|----------------------|
| S1 lvl 3             |
| S2 lvl 2             |
|----------------------|
| A: Upgrade           |
| B: Campaign          |
+----------------------+
```

---

# ⚙️ Kluczowe decyzje (ważne)

## 1. Kampanie są:

* ✔ permanentne (prościej)
* ❌ albo czasowe (bardziej skomplikowane)

👉 na Arduboy:
**bierz permanentne**

---

## 2. Brak kar (ważne)

Nie rób:

* strat za przeciążenie
* degradacji

👉 tylko:

> nie wykorzystujesz pełnego potencjału

---

## 3. Skalowanie

Możesz użyć:

```text
campaign_cost = base * (1.7 ^ count)
```

👉 ruch rośnie coraz drożej

---

# 🧠 Dlaczego to jest lepsze

✔ dodaje strategię bez UI kosztu
✔ eliminuje „spam serwerów = wygrana”
✔ tworzy naturalne decyzje
✔ łatwe do zrozumienia (2 liczby: CAP vs TRA)

---

# 🚀 Co teraz warto zrobić

1. W Twoim JS prototypie:

   * dodaj `traffic`
   * zmień income → `min(cap, traffic)`

2. Dodaj:

   * 2–3 kampanie
   * jeden przycisk do kupna

3. Test:

   * czy gracz „czuje”, że musi balansować?

---

# 💡 Opcjonalny twist (jeśli chcesz głębi)

Możesz później dodać:

* „efficiency %”:

```text
efficiency = min(cap, traffic) / max(cap, traffic)
```

i pokazać jako:

```
EFF: 80%
```

👉 bardzo czytelne, bardzo gamefeel

---

## Podsumowanie

To już nie jest tylko idle — to jest:

> **mini symulator infrastruktury**

I to jest dokładnie poziom głębi, który:

* działa na Arduboy
* nie rozwala UI
* daje sensowne decyzje
