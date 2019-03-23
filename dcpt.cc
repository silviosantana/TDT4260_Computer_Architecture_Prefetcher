#include <list>

#define MAX_DCPT_SIZE 98
#define DELTA_BUFFER_SIZE 19

#define true 1
#define false 0

using namespace std;

struct Dcpt
{
    Addr pc;
    Addr lastAddress;
    Addr lastPreftch;
    list<int> deltas;
    int deltaPointer;
};


class DCPT
{
private:
   list<Dcpt> dcpt; 
public:
    void insert_entry(Addr pc, Addr lastAddress){
        Dcpt entry;
        entry.pc = pc;
        entry.lastAddress = lastAddress;
        entry.lastPreftch = 0;
        entry.deltaPointer = 0;

        if (dcpt.size() < MAX_DCPT_SIZE)
            dcpt.push_back(entry);
        else {
            dcpt.pop_front();
            dcpt.push_back(entry);
        }
    }

    void clear (){
        dcpt.clear();
    }

    int is_present (Addr pc){
        for (list<Dcpt>::iterator it=dcpt.begin(); it != dcpt.end(); ++it){
            if (it->pc == pc)
                return true;
        }
        return false;
    }

    Dcpt get_entry(Addr pc){
        for (list<Dcpt>::iterator it=dcpt.begin(); it != dcpt.end(); ++it){
            if (it->pc == pc)
                return (*it);
        }
        Dcpt empty;
        empty.pc = 0;
        return empty;  
    }

    int add_to_delta_list (Addr pc, int delta){
        for (list<Dcpt>::iterator it=dcpt.begin(); it != dcpt.end(); ++it){
            if (it->pc == pc){
                if (it->deltas.size() < DELTA_BUFFER_SIZE){
                    it->deltas.push_back(delta);
                    it->deltaPointer++;
                }else {
                    it->deltas.pop_front();
                    it->deltas.push_back(delta);
                }
                return true;
            }   
        }
        return false;
    }

    int set_lastAddress (Addr pc, Addr ma){
        for (list<Dcpt>::iterator it=dcpt.begin(); it != dcpt.end(); ++it){
            if (it->pc == pc){
                it->lastAddress = ma;
                return true;
            }
        }
        return false;
    }

};