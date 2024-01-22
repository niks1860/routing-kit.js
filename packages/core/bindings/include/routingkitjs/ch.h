#ifndef ROUTING_KIT_JS_CH_H
#define ROUTING_KIT_JS_CH_H

#include <routingkit/contraction_hierarchy.h>
#include <emscripten/val.h>

namespace RoutingKitJS
{
  class CH
  {

  public:
    static CH build(
        unsigned node_count,
        const std::vector<unsigned> &tail,
        const std::vector<unsigned> &head,
        const std::vector<unsigned> &weight,
        emscripten::val log_message = emscripten::val::null(),
        unsigned max_pop_count = RoutingKit::ContractionHierarchy::default_max_pop_count);

    static CH build_given_rank(
        std::vector<unsigned> &rank,
        std::vector<unsigned> tail, std::vector<unsigned> head, std::vector<unsigned> weight,
        emscripten::val log_message = emscripten::val::null(),
        unsigned max_pop_count = RoutingKit::ContractionHierarchy::default_max_pop_count);

    static CH build_given_order(
        std::vector<unsigned> &order,
        std::vector<unsigned> tail, std::vector<unsigned> head, std::vector<unsigned> weight,
        emscripten::val log_message = emscripten::val::null(),
        unsigned max_pop_count = RoutingKit::ContractionHierarchy::default_max_pop_count);

    static CH read(emscripten::val read_json);

    RoutingKit::ContractionHierarchy &get() { return _ch; }
    unsigned node_count() const { return _ch.node_count(); }
    std::vector<unsigned> get_order() const { return _ch.order; }
    std::vector<unsigned> get_rank() const { return _ch.rank; }
    void write(emscripten::val write_json) const;

  private:
    RoutingKit::ContractionHierarchy _ch;

    CH(RoutingKit::ContractionHierarchy ch) : _ch(ch) {}

  }; // class CH

  class CHQuery
  {

  public:
    CHQuery(const CH &ch);

    ~CHQuery() { delete _query; }

    RoutingKit::ContractionHierarchyQuery &get() { return *_query; }

    CHQuery &reset();
    CHQuery &add_source(unsigned source, unsigned distance = 0);
    CHQuery &add_target(unsigned target, unsigned distance = 0);
    CHQuery &run();
    unsigned get_used_source();
    unsigned get_used_target();
    unsigned get_distance();
    std::vector<unsigned> get_node_path();
    std::vector<unsigned> get_arc_path();
    CHQuery &reset_source();
    CHQuery &pin_targets(std::vector<unsigned> &targets);
    CHQuery &run_to_pinned_targets();
    std::vector<unsigned> get_distances_to_targets();
    CHQuery &reset_target();
    CHQuery &pin_sources(std::vector<unsigned> &sources);
    CHQuery &run_to_pinned_sources();
    std::vector<unsigned> get_distances_to_sources();
    std::vector<unsigned> get_used_sources_to_targets();
    std::vector<unsigned> get_used_targets_to_sources();

  private:
    RoutingKit::ContractionHierarchyQuery *_query;
    CH *_ch;
  };

} // namespace RoutingKitJS

#endif // ROUTING_KIT_JS_CH_H