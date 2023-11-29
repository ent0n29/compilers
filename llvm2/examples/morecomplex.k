extern x();
extern y();
extern printval(z);
def f(x y) x*x-2*x*y+3*y*y;
def helper() f(x(),y());
def main() printval(helper());
