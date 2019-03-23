/*
 * A sample prefetcher which does sequential one-block lookahead.
 * This means that the prefetcher fetches the next block _after_ the one that
 * was just accessed. It also ignores requests to blocks already in the cache.
 */

#include "interface.hh"
#include "dcpt.cc"

DCPT dcpt;
list<Addr> candidates;
list<Addr> prefetches;
void prefetch_init(void)
{
    /* Called before any calls to prefetch_access. */
    /* This is the place to initialize data structures. */

    DPRINTF(HWPrefetch, "Initialized Simple Stride Direct Prefetcher (SDP)\n");
    //printf("My Hello world\n");

    dcpt.clear();
    candidates.clear();
    prefetches.clear();
}

void delta_correlation(Dcpt entry){
    int d1, d2;
    Addr address;

    candidates.clear();
    if (entry.deltas.size() > 1){
        list<int>::iterator it = entry.deltas.end();
        d1 = *it;
        it--;
        d2 = *it;

        address = entry.lastAddress;
        //for loop
    }
    

}

void prefetch_filter(Dcpt entry){

}

void issue_prefetches(){

}

void prefetch_access(AccessStat stat)
{
    if (stat.miss) {
        if(dcpt.is_present(stat.pc)){
            Dcpt entry = dcpt.get_entry(stat.pc);
            int delta = stat.mem_addr - entry.lastAddress; 

            if (delta != 0){
                //add delta to delta list
                dcpt.add_to_delta_list(stat.pc, delta);

                //update last address
                dcpt.set_lastAddress(stat.pc, stat.mem_addr);

                //get candidates list
                delta_correlation(entry);

                //get prefetches list (filter)
                prefetch_filter(entry);

                //issue prefetches
                issue_prefetches();
            }
           
        }else{
            dcpt.insert_entry(stat.pc, stat.mem_addr);
        }
    }
}

void prefetch_complete(Addr addr) {
    /*
     * Called when a block requested by the prefetcher has been loaded.
     */
}
