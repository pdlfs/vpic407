/* Defining this reduces produced output */
#define TRINITY_RUN
#define LOG_SYSSTAT

/* Define simulation mode: file per process or particle? */
//#define VPIC_FILE_PER_PARTICLE

/********************************/
/* Define simulation parameters */
/********************************/

/* Simulation timesteps */
#define VPIC_TIMESTEPS 1000
#define VPIC_DUMPS     20
#define VPIC_DUMP_INTERVAL (VPIC_TIMESTEPS / VPIC_DUMPS)

/* Node topology. Total nodes = x * y * z */
#define VPIC_TOPOLOGY_X 2
#define VPIC_TOPOLOGY_Y 2
#define VPIC_TOPOLOGY_Z 1

/* Particle distribution. Total particles = 100 * x * y * z */
#define VPIC_PARTICLE_X 16
#define VPIC_PARTICLE_Y 16
#define VPIC_PARTICLE_Z 1
