/*
 * dispatch.cc
 *
 *  Created on: Mar 3, 2011
 *      Author: max
 */

#include "first.hh"

#include "dispatch.hh"    // for ft_dispatch
#include "work.hh"             // for ft_work<T>

FT_NAMESPACE_BEGIN


/**
 * instantiate and run ft_work<T>
 * with the smallest T that can represent device blocks count.
 * return 0 if success, else error.
 *
 * implementation: iterates on all known configured T and,
 * if both ft_work<T>::check() and ft_work<T>::init() succeed,
 * calls ft_work<T>::run(), then ft_work<T>::quit()
 */
int ft_dispatch::main(ft_vector<ft_uoff> & loop_file_extents,
                           ft_vector<ft_uoff> & free_space_extents, FT_IO_NS ft_io & io)
{
    int err;

    if ((err = ft_work<ft_uint>::check(io)) == 0) {
        ft_work<ft_uint> worker;
        if ((err = worker.init(loop_file_extents, free_space_extents, io)) == 0)
            // if worker.init(io) succeeds, do not try any other ft_work<T>
            return worker.run();
        // worker.quit() will be called automatically by the destructor
    }

    ft_work<ft_uoff> worker;
    if ((err = worker.init(loop_file_extents, free_space_extents, io)) == 0)
        return worker.run();

    // worker.quit() will be called automatically by the destructor

    return err;
}

FT_NAMESPACE_END