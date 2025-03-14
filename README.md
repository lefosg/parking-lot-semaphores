# Parking lot with semaphores

In this repo we have a simple implementation of a parking lot.

There is a buffer which contains the mapping of the parking lot. If in a position the value is 1, then there is a parked car. If it 0, it is free space.

When we run one thread with the `park` function, it tries to find the first available position and parks.

When one thread runs the `unpark` function, its finds a parked position, and unparks it (sets the value in the buffer to 1).

To make it simpler, there is no sense of "car ownership", so if one thread finds one parked car, its marks the position free.