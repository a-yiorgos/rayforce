#!/usr/bin/env python3
import rayforce

def main():
    # Test basic arithmetic
    result = rayforce.eval("(+ 1 2)")
    print(f"1 + 2 = {result}")

    # Test list operations
    result = rayforce.eval("(list 1 2 3 4 5)")
    print(f"List: {result}")

    # Test function definition and call
    code = """
    (set factorial (fn [n]
        (if (<= n 1)
            1
            (* n (factorial (- n 1))))))
    (factorial 5)
    """
    result = rayforce.eval(code)
    print(f"Factorial of 5: {result}")

    # Test error handling
    try:
        rayforce.eval("(undefined_function)")
    except RuntimeError as e:
        print(f"Caught expected error: {e}")

if __name__ == "__main__":
    main() 