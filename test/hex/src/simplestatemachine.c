enum state{
    A,
    B,
    C
};

int main(){

    enum state s = A;
    int counter = 0;

    while(s != C){
        counter++;

        if (counter == 5){
            s = B;
        }

        if (s = B){
            counter -= 2;
        }

        if (counter == -5){
            s = C;
        }
    }
}
