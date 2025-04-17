import sys

def main():
    if len(sys.argv) < 2:
        print("Uso: python main.py <argumento1> <argumento2> ...")
        sys.exit(1)
    
    print(sys.argv[1:])

if __name__ == "__main__":
    main()

