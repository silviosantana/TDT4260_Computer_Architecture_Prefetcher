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
    Delta_t d1, d2;
    Addr address;

    candidates.clear();
    if (entry.deltas.size() > 1){
        list<Delta_t>::iterator it = entry.deltas.end();
        it--;
        d1 = *it;
        it--;
        d2 = *it;

        address = entry.lastAddress;
        //for u,v in entry deltas 
        for (list<Delta_t>::iterator u=entry.deltas.begin(); u != it; ++u){
            list<Delta_t>::iterator v = u;
            v++;
            //if u = d2 and v = d1
            if (*u == d2 && *v == d1){
                //for delta remaining in deltas
                list<Delta_t>::iterator d = v;
                for (d++; d != entry.deltas.end(); ++d){
                    address = address + *d * BLOCK_SIZE;
                    candidates.push_back(address);
                }
            }
        }
    }
}

void prefetch_filter(Dcpt entry){
    prefetches.clear();
    
    for (list<Addr>::iterator candidate = candidates.begin(); candidate != candidates.end(); ++candidate){
        /*if (*candidate == entry.lastPreftch){
            prefetches.clear();
        }*/
        
        if (!dcpt.is_in_flight(*candidate) && !in_cache(*candidate) && !in_mshr_queue(*candidate)){
            prefetches.push_back(*candidate);
            dcpt.insert_in_flight(*candidate);
            entry.lastPreftch = *candidate;
        }
    }

}

void issue_prefetches(){
    for (list<Addr>::iterator it = prefetches.begin(); it != prefetches.end(); ++it) {
        if (*it <= MAX_PHYS_MEM_ADDR){
            issue_prefetch(*it);
        }
    }  
}

void prefetch_access(AccessStat stat)
{
    //if (stat.miss) {
        if(dcpt.is_present(stat.pc)){
            Dcpt entry = dcpt.get_entry(stat.pc);
            Delta_t delta = stat.mem_addr - entry.lastAddress; 
            delta /= BLOCK_SIZE >> 1;

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
    //}
}

void prefetch_complete(Addr addr) {
    /*
     * Called when a block requested by the prefetcher has been loaded.
     */
}
