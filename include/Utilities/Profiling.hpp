#ifndef INCLUDE_UTILITIES_PROFILING_HPP_
#define INCLUDE_UTILITIES_PROFILING_HPP_

#include <chrono>
#include <fstream>
#include <mutex>
#include <thread>
#include <unordered_map>
#include "Corrade/Utility/Debug.h"
#include "Corrade/Utility/DebugAssert.h"


struct ProfileResult {
    long long start, duration = 0;
    size_t tid = 0;
};

class Profiler {
public:
    static Profiler& Instance() {
        static Profiler instance;
        return instance;
    }

    friend class ProfileSection;

private:
    void writeHeader() {
        std::unique_lock lck(this->mtx);
        fout << "{\"otherData\": {},\"traceEvents\":[";
    }
    void writeFooter() {
        std::unique_lock lck(this->mtx);
        fout << "]}";
    }

    void writeProfile(std::string name, const ProfileResult&& result) {
        std::unique_lock lck(this->mtx);

        static size_t profileCount{0};
        if (profileCount++ > 0)
            this->fout << ", ";

        std::replace(name.begin(), name.end(), '"', '\'');

        fout << "\n{";
        fout << "\"cat\":\"function\",";
        fout << "\"dur\":" << result.duration << ",";
        fout << "\"name\":\"" << name.c_str() << "\",";
        fout << "\"ph\":\"X\",";
        fout << "\"pid\":0,";
        fout << "\"tid\":" << result.tid << ",";
        fout << "\"ts\":" << result.start;
        fout << "}";
    }

    Profiler() { writeHeader(); }

    ~Profiler() { writeFooter(); }

private:
    std::ofstream fout{"profile.json"};
    std::unordered_map<std::string, ProfileResult> results;
    std::mutex mtx;
};

class ProfileSection {
public:
    ProfileSection(const std::string& sectionName) : name(sectionName) {
        auto& inst = Profiler::Instance();

        if (inst.results.contains(sectionName)) {
            Corrade::Utility::Error{} << "Tried to create profiler name " << sectionName.c_str()
                                      << " but it already exists!";
            CORRADE_ASSERT_ABORT();
        }

        using namespace std::chrono;
        auto start_time =
            time_point_cast<microseconds>(high_resolution_clock::now()).time_since_epoch().count();
        auto thread_id = std::hash<std::thread::id>{}(std::this_thread::get_id());

        inst.results[sectionName] =
            ProfileResult{.start = start_time, .duration = 0, .tid = thread_id};
    }

    void end() {
        if (!this->alive)
            return;
        this->alive = false;

        auto& inst = Profiler::Instance();

        auto result = inst.results.find(this->name);

        if (result == inst.results.end()) {
            Corrade::Utility::Error{} << "Tried to access profiler name " << name.c_str()
                                      << " but it does not exist!";
            CORRADE_ASSERT_ABORT();
        }

        auto end_time = std::chrono::time_point_cast<std::chrono::microseconds>(
                            std::chrono::high_resolution_clock::now())
                            .time_since_epoch()
                            .count();

        result->second.duration = end_time - result->second.start;

        inst.writeProfile(this->name, std::move(result->second));

        inst.results.erase(this->name);
    }

    ~ProfileSection() { this->end(); }

private:
    std::string name;
    bool alive = true;
};

#define PROFILE_SECTION(name) ProfileSection profileSection##__LINE__(name);
#define PROFILE_FUNCTION PROFILE_SECTION(__PRETTY_FUNCTION__)

#endif // INCLUDE_UTILITIES_PROFILING_HPP_
