#!/usr/bin/env python3
"""
Narzędzie do analizy balansu upgrade'ów vs capacity serwerów.
Używa tych samych formuł co gra (gameLogic.h / State.h).

Uruchomienie:
    python3 tools/balance_upgrades.py
"""

# === PARAMETRY GRY (z State.h / gameLogic.h) ===
MaxRacks = 3
RackSize = 4
MaxServerLevel = 20
serverLevelCapacityScale = 19

max_slots = MaxRacks * RackSize

SERVER_RATE = 1.3

def server_bonus(level):
    """getServerUpgradeBonus z gameLogic.h: compound 1.4x per level"""
    if level == 0:
        return 0
    bonus = serverLevelCapacityScale
    for i in range(1, level):
        bonus = int(bonus * SERVER_RATE)
    return bonus

max_capacity = max_slots * server_bonus(MaxServerLevel)

# Formuła bonusu z gameLogic.h: getOfficeUpgradeBonus()
# bonus = startingBonus; for each level: bonus += bonus * COMPOUND_RATE
COMPOUND_RATE = 0.5

upgrades = [
    {"name": "User messaging",       "price": 80,          "bonus": 10,   "max": 10},
    {"name": "Push notifications",   "price": 1000,        "bonus": 22,   "max": 10},
    {"name": "Load balancing",       "price": 3500,        "bonus": 45,   "max": 10},
    {"name": "Request routing",      "price": 12000,       "bonus": 60,   "max": 10},
    {"name": "Async processing",     "price": 40000,       "bonus": 85,   "max": 10},
    {"name": "AI core upgrade",      "price": 200000,      "bonus": 120,  "max": 5},
    {"name": "AI Context awareness", "price": 400000,      "bonus": 200,  "max": 5},
    {"name": "AI Smart responses",   "price": 4000000,     "bonus": 400,  "max": 5},
    {"name": "Predictive AI",        "price": 20000000,    "bonus": 800,  "max": 5},
    {"name": "Self evolving AI",     "price": 800000000,   "bonus": 1000, "max": 1},
]

# === FORMUŁY ===

def bonus_compound(sb, have, rate=COMPOUND_RATE):
    """Obecna formuła z gry (getOfficeUpgradeBonus)"""
    if have == 0:
        return 0
    b = sb
    for i in range(have):
        b += int(b * rate)
    return b


# === ANALIZA ===

def analyze(label, upgrade_set, bonus_func, show_steps=False):
    print(f"\n{'=' * 100}")
    print(f"  {label}")
    print(f"{'=' * 100}")

    # Podsumowanie per upgrade
    print(f"  {'#':>2} | {'Upgrade':25s} | {'Bonus':>6} | {'Max':>3} | {'Contrib':>10} | {'Cumul':>10} | {'% cap':>8}")
    cumul = 1
    for i, u in enumerate(upgrade_set):
        c = bonus_func(u["bonus"], u["max"])
        cumul += c
        pct = cumul / max_capacity * 100
        bar_len = min(int(pct / 5), 20)
        bar = "#" * bar_len + "." * (20 - bar_len)
        overflow = " OVER" if cumul > max_capacity else ""
        print(f"  {i:>2} | {u['name']:25s} | {u['bonus']:>6} | {u['max']:>3} | {c:>10,} | {cumul:>10,} | [{bar}] {pct:>5.1f}%{overflow}")
    print(f"  TOTAL: {cumul:,} / {max_capacity:,} = {cumul/max_capacity*100:.1f}%")

    # Krok po kroku
    if show_steps:
        print(f"\n  Inbound po kazdym zakupie:")
        cumul = 1
        current = [0] * len(upgrade_set)
        pnum = 0
        for i, u in enumerate(upgrade_set):
            for lvl in range(1, u["max"] + 1):
                pnum += 1
                current[i] = lvl
                cumul = 1 + sum(
                    bonus_func(upgrade_set[j]["bonus"], current[j])
                    for j in range(len(upgrade_set))
                )
                pct = cumul / max_capacity * 100
                bar_len = min(int(pct / 5), 20)
                bar = "#" * bar_len + "." * (20 - bar_len)
                overflow = " OVER" if cumul > max_capacity else ""
                print(f"    #{pnum:>3} {u['name']:25s} lvl {lvl:>2} => inbound={cumul:>6,} [{bar}] {pct:>5.1f}%{overflow}")

    return cumul


def find_overflow_point(upgrade_set, bonus_func):
    current = [0] * len(upgrade_set)
    p = 0
    for i, u in enumerate(upgrade_set):
        for lvl in range(1, u["max"] + 1):
            p += 1
            current[i] = lvl
            ti = 1 + sum(bonus_func(upgrade_set[j]["bonus"], current[j]) for j in range(len(upgrade_set)))
            if ti > max_capacity:
                print(f"\n  OVERFLOW przy: {u['name']} lvl {lvl} (zakup #{p})")
                print(f"  inbound={ti:,} > capacity={max_capacity:,}")
                return
    print(f"\n  Brak overflow - max inbound miesci sie w capacity")


# === MAIN ===

if __name__ == "__main__":
    print(f"MAX CAPACITY = {max_slots} slots x server_bonus({MaxServerLevel})={server_bonus(MaxServerLevel)} = {max_capacity}")

    print("\n" + "=" * 100)
    print("  CAPACITY na roznych etapach gry")
    print("=" * 100)
    milestones = [
        ("Start (1 srv lvl 1)",              1 * server_bonus(1)),
        ("1 rack filled (4 srv lvl 1)",       4 * server_bonus(1)),
        ("1 rack maxed (4 srv lvl 20)",       4 * server_bonus(MaxServerLevel)),
        ("3 racks filled lvl 5",             12 * server_bonus(5)),
        ("3 racks filled lvl 10",            12 * server_bonus(10)),
        ("3 racks filled lvl 15",            12 * server_bonus(15)),
        ("ALL MAXED",                         max_capacity),
    ]
    for label, cap in milestones:
        print(f"  {label:35s} => capacity = {cap:>6,}")

    # --- SYMULACJA LEVELI SERWERA ---
    print("\n" + "=" * 100)
    print("  SERVER LEVELS (1 serwer, lvl 1-20)")
    print("=" * 100)
    serverPrice = 1000
    print(f"  {'Lvl':>3} | {'Capacity':>10} | {'Gain':>6} | {'Upgrade cost':>14} | {'Cumul cost':>14} | {'Cost/cap':>10}")
    cumul_cost = 0
    prev_cap = 0
    for lvl in range(1, MaxServerLevel + 1):
        cap = server_bonus(lvl)
        gain = cap - prev_cap
        cost = int(serverPrice * (1.4 ** lvl))
        cumul_cost += cost
        cost_per_cap = cumul_cost / cap if cap > 0 else 0
        print(f"  {lvl:>3} | {cap:>10,} | +{gain:>4} | {cost:>14,} | {cumul_cost:>14,} | {cost_per_cap:>10.1f}")
        prev_cap = cap

    # --- OBECNE WARTOSCI ---
    analyze(
        "OBECNE WARTOSCI (compound 50%)",
        upgrades,
        lambda sb, h: bonus_compound(sb, h, 0.5),
        show_steps=True,
    )
    find_overflow_point(upgrades, lambda sb, h: bonus_compound(sb, h, 0.5))
