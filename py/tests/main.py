import xalpha as xa
import matplotlib

if __name__ == '__main__':
    f = xa.fundinfo('519736')
    p = f.price.plot(x='date', y=['netvalue'])
    print(f)
