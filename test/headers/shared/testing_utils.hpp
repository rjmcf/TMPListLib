#define AVOID_FAILURE_TESTS false

// Failure tests should appear between #if of the above,
// with markup of the test id in the form <[filename][number]>
// An extra
//      static_assert(false, "Checking <[filename][number]>");
// should appear directly beneath
