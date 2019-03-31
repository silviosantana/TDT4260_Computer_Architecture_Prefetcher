#include <list>
#include <cmath>

#define MAX_DCPT_SIZE 100
#define DELTA_BUFFER_SIZE 6
#define MAX_INFLIGHT_SIZE 32
#define DELTA_BITFIELD_WIDTH 16
#define MAX_DELTA_VALUE (int64_t)(pow(2, DELTA_BITFIELD_WIDTH) - 1)

#define true 1
#define false 0

using namespace std;

typedef int64_t Delta_t;

struct Dcpt
{
    Addr pc;
    Addr lastAddress;
    Addr lastPreftch;
    list<Delta_t> deltas;
    int deltaPointer;
};


class DCPT
{
private:
   list<Dcpt> dcpt;
   list<Addr> inFlight;
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

    int add_to_delta_list (Addr pc, Delta_t delta){
        if (delta < -MAX_DELTA_VALUE || delta > MAX_DELTA_VALUE){
            delta = 0;
        }
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

    void insert_in_flight(Addr candidate){
        if (inFlight.size() < MAX_INFLIGHT_SIZE)
            inFlight.push_back(candidate);
        else {
            inFlight.pop_front();
            inFlight.push_back(candidate);
        }
    }

    int is_in_flight (Addr candidate){
        for (list<Addr>::iterator it=inFlight.begin(); it != inFlight.end(); ++it){
            if (*it == candidate)
                return true;
        }
        return false;
    }

};
