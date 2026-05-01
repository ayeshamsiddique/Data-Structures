#include <iostream>
#include<string>
using namespace std;

class FullStack {};
class EmptyStack {};

class ItemType
{
public:
    ItemType()
    {
        value = '0';
    }
    void setValue(char v)
    {
        value = v;
    }
    char getValue() const
    {
        return value;
    }
    void display() const
    {
        cout << value << endl;
    }
private:
    char value;
};


const int MAX_ITEMS = 4;

class StackType
{
public:
    StackType();
    ~StackType();
    void Push(ItemType item);
    void Pop();
    bool IsFull() const;
    bool IsEmpty() const;
    ItemType Top();

private:
    int top;
    ItemType items[MAX_ITEMS];
};

StackType::StackType()
{
    top = -1;
}

StackType::~StackType() {}

void StackType::Push(ItemType item)
{
    if (IsFull())
        throw FullStack();
    top++;
    items[top] = item;
}

void StackType::Pop()
{
    if (IsEmpty())
        throw EmptyStack();
    top--;
}

ItemType StackType::Top()
{
    if (IsEmpty())
        throw EmptyStack();
    return items[top];
}

bool StackType::IsFull() const
{
    return (top == MAX_ITEMS - 1);
}

bool StackType::IsEmpty() const
{
    return (top == -1);
}

int precedence(char op)
{
    if (op == '^')
        return 3;
    if (op == '*' || op == '/')
        return 2;
    else if (op == '+' || op == '-')
        return 1;
    else
        return -1;
}

bool isOperator(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

string infixToPostfix(string infix)
{
    StackType st;
    string postfix = "";
    for (char c : infix)
    {
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c>='0' && c<='9'))
        {
            postfix += c;
        }
        else if (c == '(')
        {
            ItemType it;
            it.setValue(c);
            st.Push(it);
        }
        else if (c == ')')
        {
            while (!st.IsEmpty() && st.Top().getValue() != '(')
            {
                postfix += st.Top().getValue();
                st.Pop();
            }
            st.Pop();
        }
        else if (isOperator(c))
        {
            while (!st.IsEmpty())
            {
                char topOp = st.Top().getValue();
                if ((precedence(topOp) > precedence(c)) || (precedence(topOp) == precedence(c) && c != '^'))
                {
                    postfix += topOp;
                    st.Pop();
                }
                else break;
            }
            ItemType it;
            it.setValue(c);
            st.Push(it);
        }
    }
    while (!st.IsEmpty())
    {
        postfix += st.Top().getValue();
        st.Pop();
    }
    return postfix;
}
string infixToPrefix(string infix)
{
    reverse(infix.begin(), infix.end());
    for (int i = 0; i < infix.length(); i++) 
    {
        if (infix[i] == '(') 
            infix[i] = ')';
        else if (infix[i] == ')')
            infix[i] = '(';
    }
    string postfix = infixToPostfix(infix);
    reverse(postfix.begin(), postfix.end());
    return postfix;
}

int main()
{
    string infix;
    cout << "Enter an infix expression " << endl;
    cin >> infix;
    try
    {
        string prefix = infixToPrefix(infix);
        string postfix = infixToPostfix(infix);
        cout << "Infix: " << infix << endl;
        cout << "Prefix: " << prefix << endl;
        cout << "Postfix: " << postfix << endl;
    }
    catch (FullStack)
    {
        cout << "Error: Stack overflow" << endl;
    }
    catch (EmptyStack)
    {
        cout << "Error: Stack underflow" << endl;
    }
    return 0;
}
