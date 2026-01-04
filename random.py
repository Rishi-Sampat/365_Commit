def read_ints():
    return list(map(int, input().strip().split()))

def on_slider(x, y, slider):
    x1, y1, x2, y2 = slider
    # Must be 45-degree slope
    if abs(x2 - x1) != abs(y2 - y1):
        return False
    if min(x1, x2) <= x <= max(x1, x2) and min(y1, y2) <= y <= max(y1, y2):
        return abs(x - x1) == abs(y - y1) or abs(x - x2) == abs(y - y2)
    return False

def slide_step(x, y, slider):
    x1, y1, x2, y2 = slider
    dx = 1 if x2 > x1 else -1
    dy = 1 if y2 > y1 else -1
    # Slide along the slope
    if (x, y) == (x1, y1):
        return x + dx, y + dy
    if (x, y) == (x2, y2):
        return x - dx, y - dy
    return x + dx, y + dy

def fall_down(x, y, sliders):
    next_y = 0  # ground by default
    for s in sliders:
        x1, y1, x2, y2 = s
        if min(x1, x2) <= x <= max(x1, x2):
            slope = 1 if y2 - y1 == x2 - x1 else -1
            y_at_x = y1 + slope * (x - x1)
            if y_at_x < y and y_at_x > next_y:
                next_y = y_at_x
    return x, next_y

def solve():
    N = int(input())
    sliders = [tuple(read_ints()) for _ in range(N)]
    x, y = read_ints()

    while True:
        moved = False
        # Slide along a slider if on one
        for s in sliders:
            if on_slider(x, y, s):
                x, y = slide_step(x, y, s)
                moved = True
                break
        if not moved:
            # Fall straight down
            x_new, y_new = fall_down(x, y, sliders)
            if y_new == y:
                # Cannot move further (either stuck or reached ground)
                print(int(x), int(y))
                return
            y = y_new

if __name__ == "__main__":
    solve()
