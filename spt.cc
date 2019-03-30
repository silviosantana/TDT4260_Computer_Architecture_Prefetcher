#include <list>

#define MAX_SPT_SIZE 256

#define true 1
#define false 0

enum State {INITIAL, TRANSIENT, STEADY, NOPRED};

using namespace std;

struct Spt
{
    Addr pc;
    Addr ma;
    int valid;
    int64_t stride;
    State state;
};


class SPT
{
private:
   list<Spt> spt; 
public:
    void insert_entry(Addr pc, Addr ma){
        Spt entry;
        entry.pc = pc;
        entry.ma = ma;
        entry.valid = true;
        entry.stride = 0;
        entry.state = INITIAL;

        if (spt.size() < MAX_SPT_SIZE)
            spt.push_back(entry);
        else {
            spt.pop_front();
            spt.push_back(entry);
        }
    }

    void clear (){
        spt.clear();
    }

    Spt get_entry(Addr pc){
        for (list<Spt>::iterator it=spt.begin(); it != spt.end(); ++it){
            if (it->pc == pc)
                return (*it);
        }
        Spt empty;
        empty.pc = 0;
        return empty;  
    }

    int is_present (Addr pc){
        for (list<Spt>::iterator it=spt.begin(); it != spt.end(); ++it){
            if (it->pc == pc)
                return true;
        }
        return false;
    }

    int set_ma (Addr pc, Addr ma){
        for (list<Spt>::iterator it=spt.begin(); it != spt.end(); ++it){
            if (it->pc == pc){
                it->ma = ma;
                return true;
            }
        }
        return false;
    }

    Addr get_ma (Addr pc){
        for (list<Spt>::iterator it=spt.begin(); it != spt.end(); ++it){
            if (it->pc == pc){
                return it->ma;
            }
        }
        return false;
    }

    int get_state (Addr pc){
        for (list<Spt>::iterator it=spt.begin(); it != spt.end(); ++it){
            if (it->pc == pc){
                return it->state;
            }
        }
        return false;
    }

    int set_state (Addr pc, State state){
        for (list<Spt>::iterator it=spt.begin(); it != spt.end(); ++it){
            if (it->pc == pc){
                it->state = state;
                return true;
            }
        }
        return false;
    }

    int64_t get_stride (Addr pc){
        for (list<Spt>::iterator it=spt.begin(); it != spt.end(); ++it){
            if (it->pc == pc){
                return it->stride;
            }
        }
        return false;
    }

    int set_stride (Addr pc, int64_t stride){
        for (list<Spt>::iterator it=spt.begin(); it != spt.end(); ++it){
            if (it->pc == pc){
                it->stride = stride;
                return true;
            }
        }
        return false;
    }
};
