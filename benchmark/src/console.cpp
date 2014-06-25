/**
 * Copyright (c) 2014  Koen Visscher, Mick van Duijn and Paul Visscher
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

#include "benchmark/console.h"

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <stdio.h>

void SetColour( WORD colour )
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

void BenchLib::Console::Init( std::size_t testCount, std::size_t groupCount )
{
    PrintHeader();
    std::cout << "Running " <<  GetBenchmarkAmount( testCount ) << " from " << GetGroupAmount(
                  groupCount ) << "." << std::endl;
}

void BenchLib::Console::GroupStart( const std::string &group, std::size_t groupSize )
{
    PrintSubheader();
    std::cout << GetBenchmarkAmount( groupSize ) << " from " << group << std::endl;
}

void BenchLib::Console::NormalStart( std::size_t groupSize )
{
    PrintHeader();
    std::cout << std::endl;

    PrintSubheader();
    std::cout << "Normal benchmarks (" <<groupSize << "):" << std::endl;
}

void BenchLib::Console::Run( const std::string &group, const std::string &name )
{
    PrintRun();
    std::cout << group << "." << name << std::endl;
}

void BenchLib::Console::Stats( std::size_t samples, std::size_t operations )
{
    PrintStats();
    std::cout << "Samples: " << samples << "\tOperations: " << operations << ";\tTimes in ms/operation." << std::endl;
}

void BenchLib::Console::Baseline( double correction )
{
    PrintBaseline();
    std::cout << "Framework noise: " << correction << std::endl;
}


void BenchLib::Console::Result( double avg, double sd, double low, double high )
{
    PrintResult();
    std::cout << "AVG: " << avg << "\tSD: " << sd << "\tLOW: " << low  << "\tHIGH: " << high <<
              std::endl;
}

void BenchLib::Console::ResultCorrected( double avg, double sd, double low, double high,
        const double baseline )
{
    PrintResultCorrected();
    std::cout << "AVG: " << avg - baseline << "\tSD: " << sd << "\tLOW: " << low  - baseline <<
              "\tHIGH: " << high  - baseline << std::endl;
}

void BenchLib::Console::Fail( const std::string &group, const std::string &name )
{
    PrintFail();
    std::cout << group << "." << name << std::endl;
}

void BenchLib::Console::Done( const std::string &group, const std::string &name, uint64_t benchTime,
                               uint64_t baselineTime )
{
    PrintDone();
    std::cout << group << "." << name << " (" << benchTime << " + " << baselineTime << "; " << benchTime + baselineTime <<
              " ms)" << std::endl;
}

void BenchLib::Console::GroupEnd( const std::string &group, std::size_t groupSize, uint64_t totalTime )
{
    PrintSubheader();
    std::cout << GetBenchmarkAmount( groupSize ) << " from " << group << " (" << totalTime << " ms total)\n" << std::endl;
}

void BenchLib::Console::End( std::size_t totalBenchmarks, std::size_t totalGroups, uint64_t totalTime,
                              const std::vector< std::pair< std::string, std::string > > &failed )
{
    PrintSubheader();
    std::cout << "Global benchmark environment tear-down" << std::endl;

    PrintHeader();
    std::cout << GetBenchmarkAmount( totalBenchmarks ) << " from " << GetGroupAmount( totalGroups ) << " ran. (" <<
              totalTime << " ms total)" << std::endl;

    std::size_t totalFailedBenchmarks = failed.size();

    PrintCompleted();
    std::cout << GetBenchmarkAmount( totalBenchmarks - totalFailedBenchmarks ) << "." << std::endl;

    if ( totalFailedBenchmarks > 0 )
    {
        PrintFail();
        std::cout << GetBenchmarkAmount( totalFailedBenchmarks ) << ", listed below:" << std::endl;

        for ( auto it = failed.begin(), end = failed.end(); it != end; ++it )
        {
            PrintFail();
            const std::pair< std::string, std::string > &bench = *it;
            std::cout << bench.first << "." << bench.second << std::endl;
        }
    }
}

void BenchLib::Console::PrintHeader()
{
    SetGreenColour();
    std::cout << "[============] ";
    SetDefaultColour();
}

void BenchLib::Console::PrintSubheader()
{
    SetGreenColour();
    std::cout << "[------------] ";
    SetDefaultColour();
}

void BenchLib::Console::PrintRun()
{
    SetGreenColour();
    std::cout << "[ RUN        ] ";
    SetDefaultColour();
}

void BenchLib::Console::PrintStats()
{
    SetCyanColour();
    std::cout << "[  STATS     ] ";
    SetDefaultColour();
}

void BenchLib::Console::PrintBaseline()
{
    SetCyanColour();
    std::cout << "[  BASELINE  ] ";
    SetDefaultColour();
}

void BenchLib::Console::PrintResult()
{
    SetGreenColour();
    std::cout << "[   RESULT   ] ";
    SetDefaultColour();
}

void BenchLib::Console::PrintResultCorrected()
{
    SetGreenColour();
    std::cout << "[   RESULT*  ] ";
    SetDefaultColour();
}

void BenchLib::Console::PrintDone()
{
    SetGreenColour();
    std::cout << "[       DONE ] ";
    SetDefaultColour();
}

void BenchLib::Console::PrintCompleted()
{
    SetGreenColour();
    std::cout << "[  COMPLETED ] ";
    SetDefaultColour();
}

void BenchLib::Console::PrintFail()
{
    SetRedColour();
    std::cout << "[   FAILED   ] ";
    SetDefaultColour();
}

void BenchLib::Console::SetDefaultColour()
{
#ifdef _WIN32
    SetColour( FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE );
#else
    std::cout << "\033[0m";
#endif
}

void BenchLib::Console::SetRedColour()
{
#ifdef _WIN32
    SetColour( FOREGROUND_RED | FOREGROUND_INTENSITY );
#else
    std::cout << "\033[0;31m";
#endif
}

void BenchLib::Console::SetGreenColour()
{
#ifdef _WIN32
    SetColour( FOREGROUND_GREEN | FOREGROUND_INTENSITY );
#else
    std::cout << "\033[0;32m";
#endif
}

void BenchLib::Console::SetBlueColour()
{
#ifdef _WIN32
    SetColour( FOREGROUND_BLUE | FOREGROUND_INTENSITY );
#else
    std::cout << "\033[0;34m";
#endif
}

void BenchLib::Console::SetCyanColour()
{
#ifdef _WIN32
    SetColour( FOREGROUND_BLUE | FOREGROUND_GREEN );
#else
    std::cout << "\033[0;36m";
#endif
}

std::string BenchLib::Console::GetBenchmarkAmount( std::size_t amount )
{
    return std::to_string( amount ) + std::string( amount > 1 ? " benchmarks" : " benchmark" );
}

std::string BenchLib::Console::GetGroupAmount( std::size_t amount )
{
    return std::to_string( amount ) + std::string( amount > 1 ? " groups" : " group" );
}
