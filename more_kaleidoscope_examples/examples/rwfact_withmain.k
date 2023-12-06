extern n();
extern printval(y);
def fact(n) n<1 ? 1 : n*fact(n-1);
def main() printval(fact(n()));
