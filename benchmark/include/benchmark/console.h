/**
 * Copyright (c) 2017 Zefiros Software.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#pragma once
#ifndef __BENCHLIB__CONSOLE_H__
#define __BENCHLIB__CONSOLE_H__

#include "benchmark/benchmarkStat.h"

#include <algorithm>
#include <iostream>
#include <stdint.h>
#include <iomanip>
#include <chrono>
#include <vector>
#include <string>

#ifdef _WIN32
#include <windows.h>
#include <stdio.h>

static void SetColour( WORD colour )
{
    HANDLE handle = GetStdHandle( STD_OUTPUT_HANDLE );

    if ( handle != INVALID_HANDLE_VALUE )
    {
        SetConsoleTextAttribute( handle, colour );
    }
}


#else
#include <iostream>
#include <curses.h>
#endif

namespace BenchLib
{
    namespace Console
    {
        const std::size_t gPrecision = 6;

        inline void SetDefaultColour()
        {
#ifdef _WIN32
            SetColour( FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE );
#else
            std::cout << "\033[0m";
#endif
        }

        inline void SetRedColour()
        {
#ifdef _WIN32
            SetColour( FOREGROUND_RED | FOREGROUND_INTENSITY );
#else
            std::cout << "\033[0;31m";
#endif
        }

        inline void SetGreenColour()
        {
#ifdef _WIN32
            SetColour( FOREGROUND_GREEN | FOREGROUND_INTENSITY );
#else
            std::cout << "\033[0;32m";
#endif
        }

        inline void SetBlueColour()
        {
#ifdef _WIN32
            SetColour( FOREGROUND_BLUE | FOREGROUND_INTENSITY );
#else
            std::cout << "\033[0;34m";
#endif
        }

        inline void SetCyanColour()
        {
#ifdef _WIN32
            SetColour( FOREGROUND_BLUE | FOREGROUND_GREEN );
#else
            std::cout << "\033[0;36m";
#endif
        }

        inline void SetMagentaColour()
        {
#ifdef _WIN32
            SetColour( FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY );
#else
            std::cout << "\033[0;95m";
#endif
        }

        inline void PrintHeader()
        {
            SetGreenColour();
            std::cout << "[============] ";
            SetDefaultColour();
        }

        inline void PrintSubheader()
        {
            SetGreenColour();
            std::cout << "[------------] ";
            SetDefaultColour();
        }

        inline void PrintRun()
        {
            SetGreenColour();
            std::cout << "[ RUN        ] ";
            SetDefaultColour();
        }

        inline void PrintStats()
        {
            SetCyanColour();
            std::cout << "[  STATS     ] ";
            SetDefaultColour();
        }

        inline void PrintBaseline()
        {
            SetCyanColour();
            std::cout << "[  BASELINE  ] ";
            SetDefaultColour();
        }

        inline void PrintResult()
        {
            SetGreenColour();
            std::cout << "[   RESULT   ] ";
            SetDefaultColour();
        }

        inline void PrintResultCorrected()
        {
            SetGreenColour();
            std::cout << "[   RESULT*  ] ";
            SetDefaultColour();
        }

        inline void PrintDone()
        {
            SetGreenColour();
            std::cout << "[       DONE ] ";
            SetDefaultColour();
        }

        inline void PrintCompleted()
        {
            SetGreenColour();
            std::cout << "[  COMPLETED ] ";
            SetDefaultColour();
        }

        inline void PrintFail()
        {
            SetRedColour();
            std::cout << "[   FAILED   ] ";
            SetDefaultColour();
        }

        inline void PrintRegressed()
        {
            SetMagentaColour();
            std::cout << "[  REGRESSED ] ";
            SetDefaultColour();
        }

        inline std::string GetBenchmarkAmount( std::size_t amount )
        {
            return std::to_string( amount ) + std::string( amount > 1 ? " benchmarks" : " benchmark" );
        }

        inline std::string GetGroupAmount( std::size_t amount )
        {
            return std::to_string( amount ) + std::string( amount > 1 ? " groups" : " group" );
        }

        inline void Init( std::size_t testCount, std::size_t groupCount )
        {
            PrintHeader();
            std::cout << "Running " <<  GetBenchmarkAmount( testCount ) << " from " << GetGroupAmount(
                          groupCount ) << "." << std::endl;
        }

        inline void GroupStart( const std::string &group, std::size_t groupSize )
        {
            PrintSubheader();
            std::cout << GetBenchmarkAmount( groupSize ) << " from " << group << std::endl;
        }

        inline void BenchTypeStart( const std::string &benchType, std::size_t groupSize )
        {
            PrintHeader();
            std::cout << std::endl;

            PrintSubheader();
            std::cout << benchType << " Benchmarks (" << groupSize << "):" << std::endl;
        }

        inline void Run( const std::string &group, const std::string &name )
        {
            PrintRun();
            std::cout << group << "." << name << std::endl;
        }

        inline void Stats( std::size_t samples, std::size_t operations )
        {
            PrintStats();
            std::cout << "Samples: " << samples << "\tOperations: " << operations << ";\tTimes in ms/operation." << std::endl;
        }

        inline void Baseline( double correction )
        {
            PrintBaseline();
            std::cout << "Framework noise: " <<  std::fixed << std::setprecision( gPrecision ) << correction << std::endl;
        }

        inline void RegressTimeSlower( double lower, double upper, double average )
        {
            PrintRegressed();
            std::cout << "Ran slower than expected:\tExpected Range: ["
                      << std::fixed << std::setprecision( gPrecision ) << lower << ", "
                      << std::fixed << std::setprecision( gPrecision ) << upper << "]\tLower Bound: "
                      << std::fixed << std::setprecision( gPrecision ) << average
                      << std::endl;
        }

        inline void RegressTimeFaster( double lower, double upper, double average )
        {
            PrintRegressed();
            std::cout << "Ran faster than expected:\tExpected Range: ["
                      << std::fixed << std::setprecision( gPrecision ) << lower << ", "
                      << std::fixed << std::setprecision( gPrecision ) << upper << "]\tUpper Bound: "
                      << std::fixed << std::setprecision( gPrecision ) << average
                      << std::endl;
        }

        inline void RegressMemSmaller( double lower, double upper, double average )
        {
            PrintRegressed();
            std::cout << "Used less memory than expected:\tExpected Range: ["
                      << std::fixed << std::setprecision( gPrecision ) << lower << ", "
                      << std::fixed << std::setprecision( gPrecision ) << upper << "]\tAVG: "
                      << std::fixed << std::setprecision( gPrecision ) << average
                      << std::endl;
        }

        inline void RegressMemLarger( double lower, double upper, double average )
        {
            PrintRegressed();
            std::cout << "Used more memory than expected:\tExpected Range: ["
                      << std::fixed << std::setprecision( gPrecision ) << lower << ", "
                      << std::fixed << std::setprecision( gPrecision ) << upper << "]\tAVG: "
                      << std::fixed << std::setprecision( gPrecision ) << average
                      << std::endl;
        }

        inline void RegressPeakMemLarger( int64_t lower, int64_t upper, std::size_t actual )
        {
            PrintRegressed();
            std::cout << "Higher peak memory than expected:\tExpected Range: ["
                      << lower << ", "
                      << std::fixed << std::setprecision( gPrecision ) << upper << "]\tActual: " << actual
                      << std::endl;
        }

        inline void RegressPeakMemSmaller( int64_t lower, int64_t upper, std::size_t actual )
        {
            PrintRegressed();
            std::cout << "Lower peak memory than expected:\tExpected Range: ["
                      << lower << ", "
                      << std::fixed << std::setprecision( gPrecision ) << upper << "]\tActual: " << actual
                      << std::endl;
        }

        template< typename tT, typename tSample >
        inline void Result( BenchmarkStat< tT, tSample > stats )
        {
            std::cout << "AVG: " << std::fixed << std::setprecision( gPrecision ) << stats.average <<
                      "\tSD: " << std::fixed << std::setprecision( gPrecision ) << stats.standardDeviation <<
                      "\tLOW: " << std::fixed << std::setprecision( gPrecision ) << stats.low  <<
                      "\tHIGH: " << std::fixed << std::setprecision( gPrecision ) << stats.high <<
                      std::endl;
        }

        inline void Fail( const std::string &group, const std::string &name )
        {
            PrintFail();
            std::cout << group << "." << name << std::endl;
        }

        inline void Done( const std::string &group, const std::string &name, double benchTime, double baselineTime )
        {
            PrintDone();
            std::cout << group << "." << name << " Unit Time: (" << benchTime << " + " << baselineTime << "; " <<
                      benchTime + baselineTime << " msec)" << std::endl;
        }

        inline void GroupEnd( const std::string &group, std::size_t groupSize, std::chrono::milliseconds totalTime )
        {
            PrintSubheader();
            std::cout << GetBenchmarkAmount( groupSize ) << " from " << group << " (" << totalTime.count() << " ms total)\n" <<
                      std::endl;
        }

        inline void End( std::size_t totalBenchmarks, std::size_t totalGroups, std::chrono::milliseconds totalTime,
                         const std::vector< std::pair< std::string, std::string > > &failed,
                         const std::vector< std::pair< std::string, std::string > > &regressed )
        {
            PrintSubheader();
            std::cout << "Global benchmark environment tear-down" << std::endl;

            PrintHeader();
            std::cout << GetBenchmarkAmount( totalBenchmarks ) << " from " << GetGroupAmount( totalGroups ) << " ran. (" <<
                      totalTime.count() << " ms total)" << std::endl;

            const std::size_t totalFailedBenchmarks = failed.size();

            PrintCompleted();
            std::cout << GetBenchmarkAmount( totalBenchmarks - totalFailedBenchmarks ) << "." << std::endl;

            if ( totalFailedBenchmarks > 0 )
            {
                PrintFail();
                std::cout << GetBenchmarkAmount( totalFailedBenchmarks ) << ", listed below:" << std::endl;

                for ( auto &it : failed )
                {
                    PrintFail();
                    std::cout << it.first << "." << it.second << std::endl;
                }
            }


            const std::size_t totalRegressedBenchmarks = regressed.size();

            if ( totalRegressedBenchmarks > 0 )
            {
                PrintRegressed();
                std::cout << GetBenchmarkAmount( totalRegressedBenchmarks ) << ", listed below:" << std::endl;

                for ( auto &it : regressed )
                {
                    PrintRegressed();
                    std::cout << it.first << "." << it.second << std::endl;
                }
            }

            if ( totalFailedBenchmarks > 0 )
            {
                std::cout << totalFailedBenchmarks << " FAILED " << ( ( totalFailedBenchmarks > 1 ) ? "BENCHMARKS" : "BENCHMARK" ) <<
                          std::endl;
            }

            if ( totalRegressedBenchmarks > 0 )
            {
                std::cout << totalRegressedBenchmarks << " REGRESSED " << ( ( totalRegressedBenchmarks > 1 ) ? "BENCHMARKS" :
                                                                            "BENCHMARK" ) << std::endl;
            }
        }

    }
}

#endif