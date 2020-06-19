#pragma once

#include "kernel.h"
#include "kpair.h"

namespace ral {
namespace cache {

/**
	@brief A class that represents the execution graph in a taskflow scheme.
	The taskflow scheme is basically implemeted by the execution graph and the kernels associated to each node in the graph.
*/
class graph {
protected:
	struct Edge {
		std::int32_t source;
		std::int32_t target;
		std::string source_port_name;
		std::string target_port_name;

		bool operator<(const Edge & e) const { return this->target < e.target || (this->target == e.target && this->source < e.source); }
		bool operator==(const Edge & e) const { return this->target == e.target && this->source == e.source; }

		void print() const {
			std::cout<<"Edge: source id: "<<source<<" name: "<<source_port_name<<" target id: "<<target<<" name: "<<target_port_name<<std::endl;
		}
	};

public:
	graph() {
		container_[head_id_] = nullptr;	 // sentinel node
		kernels_edges_logger = spdlog::get("kernels_edges_logger");
	}
	graph(const graph &) = default;
	graph & operator=(const graph &) = default;

	void addPair(kpair p);

	void check_and_complete_work_flow();

	void execute();

	void show();

	void show_from_kernel (int32_t id);

	std::pair<bool, uint64_t> get_estimated_input_rows_to_kernel(int32_t id);

	std::pair<bool, uint64_t> get_estimated_input_rows_to_cache(int32_t id, const std::string & port_name);

	std::shared_ptr<kernel> get_last_kernel();

	size_t num_nodes() const;

	size_t add_node(std::shared_ptr<kernel> k);

	void add_edge(std::shared_ptr<kernel> source,
		std::shared_ptr<kernel> target,
		std::string source_port,
		std::string target_port,
		const cache_settings & config);

	kernel * get_node(size_t id);

	std::set<Edge> get_neighbours(kernel * from);
	std::set<Edge> get_neighbours(int32_t id);
	std::set<Edge> get_reverse_neighbours(kernel * from);
	std::set<Edge> get_reverse_neighbours(int32_t id);

	void check_for_simple_scan_with_limit_query();

private:
	const std::int32_t head_id_{-1};
	std::vector<kernel *> kernels_;
	std::map<std::int32_t, std::shared_ptr<kernel>> container_;
	std::map<std::int32_t, std::set<Edge>> edges_;
	std::map<std::int32_t, std::set<Edge>> reverse_edges_;

	std::shared_ptr<spdlog::logger> kernels_edges_logger;
};


}  // namespace cache
}  // namespace ral
