#include "ttt.h"

u64 armv8_pmccntr_read(void)
{
        u64 val;
	isb();
	asm volatile("mrs %0, pmccntr_el0" : "=r" (val));
        return val;
}
void pmu_pause(void)
{
	asm volatile("msr pmcntenclr_el0, %0" :: "r"((u64)(1 << 31)));
}
void pmu_continue(void)
{
	asm volatile("msr pmcntenset_el0, %0" :: "r"((u64)(1 << 31)));
}

u32 pmu_filt_read(void)
{
        u32 val=0;
	asm volatile("mrs %0, pmccfiltr_el0" : "=r" (val));
        return val;

}
void pmu_filt_write(void)
{
	asm volatile("msr pmccfiltr_el0, %0" :: "r"((u32)(1 << 31)));

}

unsigned int armv8_pmu_pmcr_read(void)
{
	unsigned int val;
	asm volatile("mrs %0, pmcr_el0" : "=r" (val));
	return val;
}

void armv8_pmu_pmcr_write(unsigned int val)
{
    asm volatile("msr pmcr_el0, %0" : :"r" (val & ARMV8_PMCR_MASK));
}


void enable_all_counters(unsigned int evtCount)
{
    unsigned int val;
    /* Enable all counters */
    val = armv8_pmu_pmcr_read();
    val |= QUADD_ARMV8_PMCR_E | QUADD_ARMV8_PMCR_X;
    armv8_pmu_pmcr_write(val);
    asm volatile("isb");
    /* Just use counter 0 here */
    asm volatile("msr pmevtyper0_el0, %0" : : "r" (evtCount));
    /*   Performance Monitors Count Enable Set register bit 30:1 disable, 31,1 enable */
    unsigned int r = 0;
    asm volatile("mrs %0, pmcntenset_el0" : "=r" (r));
    asm volatile("msr pmcntenset_el0, %0" : : "r" (r|1));
}

void reset_all_counters(void)
{

   unsigned int val;
    val = armv8_pmu_pmcr_read();
    val |= QUADD_ARMV8_PMCR_P | QUADD_ARMV8_PMCR_C;
    armv8_pmu_pmcr_write(val);
}

unsigned int enabled=0;

