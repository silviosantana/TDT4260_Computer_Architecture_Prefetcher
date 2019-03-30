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
    //if (stat.miss) {
        if(spt.is_present(stat.pc)){
            Spt entry = spt.get_entry(stat.pc);
            int64_t newStride = stat.mem_addr - entry.ma; 
            Addr newMemAddr = stat.mem_addr + newStride;

            int should_fetch = 0;

            switch(entry.state)
            {
                case INITIAL  : 
                    if (newStride == entry.stride){
                        spt.set_state(stat.pc, STEADY);
                        spt.set_ma(stat.pc, stat.mem_addr);
                    } else {
                        spt.set_state(stat.pc, TRANSIENT);
                        spt.set_ma(stat.pc, stat.mem_addr);
                        spt.set_stride(stat.pc, newStride);
                    } 
                    should_fetch = 1;
                    break;
                case TRANSIENT: 
                    if (newStride == entry.stride){
                        spt.set_state(stat.pc, STEADY);
                        spt.set_ma(stat.pc, stat.mem_addr);
                    } else {
                        spt.set_state(stat.pc, NOPRED);
                        spt.set_ma(stat.pc, stat.mem_addr);
                        spt.set_stride(stat.pc, newStride);
                    } 
                    should_fetch = 1;
                    break;
                case STEADY : 
                    if (newStride == entry.stride){
                        spt.set_ma(stat.pc, stat.mem_addr);
                    } else {
                        spt.set_state(stat.pc, INITIAL);
                        spt.set_ma(stat.pc, stat.mem_addr);
                    } 
                    should_fetch = 1;  
                    break;
                case NOPRED : 
                    if (newStride == entry.stride){
                        spt.set_state(stat.pc, TRANSIENT);
                        spt.set_ma(stat.pc, stat.mem_addr);
                    } else {
                        spt.set_ma(stat.pc, stat.mem_addr);
                        spt.set_stride(stat.pc, newStride);
                    } 
                    should_fetch = 0;  
                    break;
            }
                        
            if (should_fetch == 1 && newMemAddr <= MAX_PHYS_MEM_ADDR && !in_cache(newMemAddr) && !in_mshr_queue(newMemAddr)){
                issue_prefetch(newMemAddr);
            }  
        }else{
            spt.insert_entry(stat.pc, stat.mem_addr);
        }
    //}
}

void prefetch_complete(Addr addr) {
    /*
     * Called when a block requested by the prefetcher has been loaded.
     */
}
