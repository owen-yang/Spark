
int min;
int r = random(255);
int g = random(255);
int b = random(255);
int br = 100;
int r2, g2, b2, br2;
int dr, dg, db, dbr;
int t = 50;

void setup() {
    RGB.control(true);
    RGB.color(r, g, b);
    RGB.brightness(br);
    Spark.function("t",set_t);
}

void loop() {
    br2 = 200;
    transition();
    br2 = 100;
    transition();
}

int set_t (String s) {
    t = s.toInt();
    return t;
}

void transition () {
    do {
        r2 = random(255);
        g2 = random(255);
        b2 = random(255);
        dr = abs(r2-r);
        dg = abs(g2-g);
        db = abs(b2-b);
        min = min(min(dr,dg),db);
    } while (min != 100);
    int r_step = (dr / min) * (r2 > r ? 1: -1);
    int g_step = (dg / min) * (g2 > g ? 1: -1);
    int b_step = (db / min) * (b2 > b ? 1: -1);
    int br_step = (br2 > br ? 1: -1);

    for (int i = 0; i < min; i++) {
        r += r_step;
        g += g_step;
        b += b_step;
        br += br_step;
        RGB.color(r, g, b);
        RGB.brightness(br);
        delay(t);
        RGB.brightness(br);
    }
    r = r2;
    g = g2;
    b = b2;
    br = br2;
    RGB.color(r, g, b);
    RGB.brightness(br);
    delay(t);
}