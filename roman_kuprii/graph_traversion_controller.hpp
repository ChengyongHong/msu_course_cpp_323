#pragma once

#include <atomic>
#include <functional>
#include <list>
#include <mutex>
#include <optional>
#include <thread>

#include "graph_traverser.hpp"

namespace uni_cpp_practice {

class Graph;

namespace graph_traversion_controller {

class GraphTraversionController {
 public:
  using JobCallback = std::function<void()>;
  using GetJobCallback = std::function<std::optional<JobCallback>()>;
  using GenStartedCallback = std::function<void(int)>;
  using GenFinishedCallback =
      std::function<void(int, const std::vector<GraphTraverser::Path>&)>;

  class Worker {
   public:
    explicit Worker(const GetJobCallback& get_job_callback)
        : get_job_callback_(get_job_callback){};

    enum class State { Idle, Working, ShouldTerminate };

    void start();
    void stop();

    ~Worker();

   private:
    std::thread thread_;
    GetJobCallback get_job_callback_;
    std::atomic<State> state_ = State::Idle;
  };

  GraphTraversionController(int threads_count, int graphs_count);

  void traverse_graphs(const std::vector<Graph>& graphs,
                       const GenStartedCallback& gen_started_callback,
                       const GenFinishedCallback& gen_finished_callback);

 private:
  std::list<Worker> workers_;
  std::list<JobCallback> jobs_;
  int graphs_count_;
  std::mutex start_callback_mutex_;
  std::mutex finish_callback_mutex_;
  std::mutex get_job_mutex_;
};

}  // namespace graph_traversion_controller

}  // namespace uni_cpp_practice
