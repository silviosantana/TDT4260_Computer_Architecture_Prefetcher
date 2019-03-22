/*
 * A sample prefetcher which does sequential one-block lookahead.
 * This means that the prefetcher fetches the next block _after_ the one that
 * was just accessed. It also ignores requests to blocks already in the cache.
 */

#include "interface.hh"
#include "spt.cc"

SPT spt;
void prefetch_init(void)
{
    /* Called before any calls to prefetch_access. */
    /* This is the place to initialize data structures. */

    DPRINTF(HWPrefetch, "Initialized Simple Stride Direct Prefetcher (SDP)\n");
    //printf("My Hello world\n");

    spt.clear();
}

void prefetch_access(AccessStat stat)
{
    if (stat.miss) {
        if(spt.is_present(stat.pc)){
            Addr last_ma = spt.get_ma(stat.pc);
            int stride = stat.mem_addr - last_ma;
            Addr newMemAddr = stat.mem_addr + stride;
            if (stride != 0 && newMemAddr <= MAX_PHYS_MEM_ADDR && !in_cache(newMemAddr) && !in_mshr_queue(newMemAddr)){
                issue_prefetch(newMemAddr);
                spt.set_ma(stat.pc, stat.mem_addr);
            }  
        }else{
            spt.insert_entry(stat.pc, stat.mem_addr);
        }
    }
}

void prefetch_complete(Addr addr) {
    /*
     * Called when a block requested by the prefetcher has been loaded.
     */
}
