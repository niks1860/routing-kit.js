#ifndef ROUTING_KIT_JS_CCH_H
#define ROUTING_KIT_JS_CCH_H

#include <routingkit/customizable_contraction_hierarchy.h>
#include <emscripten/val.h>

namespace RoutingKitJS
{
  class CCH;

  struct CCH
  {
  public:
    CCH(
        unsigned node_count,
        const std::vector<unsigned> &tail,
        const std::vector<unsigned> &head,
        const std::vector<float> &lat,
        const std::vector<float> &lon,
        emscripten::val log_message = emscripten::val::null());

    ~CCH() { delete _cch; }

    RoutingKit::CustomizableContractionHierarchy &get() { return *_cch; }
    unsigned node_count() const { return _cch->node_count(); }
    unsigned input_arc_count() const { return _cch->input_arc_count(); }
    unsigned cch_arc_count() const { return _cch->cch_arc_count(); }

  private:
    RoutingKit::CustomizableContractionHierarchy *_cch;

  }; // class CCH

  class CCHMetric;

  struct CCHMetric
  {
  public:
    CCHMetric(
        const CCH &cch,
        const std::vector<unsigned> &weight);

    ~CCHMetric() { delete _metric; }

    RoutingKit::CustomizableContractionHierarchyMetric &get() { return *_metric; }
    std::vector<unsigned> &get_weights() { return *_weights; }

    CCHMetric &reset(const std::vector<unsigned> &weight);
    CCHMetric &customize();

  private:
    CCH *_cch;
    RoutingKit::CustomizableContractionHierarchyMetric *_metric;
    std::vector<unsigned> *_weights;

  }; // class CChMetric

  class CCHQuery;

  struct CCHQuery
  {
  public:
    CCHQuery(const CCHMetric &metric);

    ~CCHQuery() { delete _query; }

    RoutingKit::CustomizableContractionHierarchyQuery &get() { return *_query; }

    CCHQuery &reset();
    CCHQuery &add_source(unsigned source, unsigned distance = 0);
    CCHQuery &add_target(unsigned target, unsigned distance = 0);
    CCHQuery &run();
    unsigned get_used_source();
    unsigned get_used_target();
    unsigned get_distance();
    std::vector<unsigned> get_node_path();
    std::vector<unsigned> get_arc_path();
    CCHQuery &reset_source();
    CCHQuery &pin_targets(std::vector<unsigned> &targets);
    CCHQuery &run_to_pinned_targets();
    std::vector<unsigned> get_distances_to_targets();
    CCHQuery &reset_target();
    CCHQuery &pin_sources(std::vector<unsigned> &sources);
    CCHQuery &run_to_pinned_sources();
    std::vector<unsigned> get_distances_to_sources();

  private:
    CCHMetric *_metric;
    RoutingKit::CustomizableContractionHierarchyQuery *_query;

  }; // class CCHQuery

} // namespace RoutingKitJS

#endif // ROUTING_KIT_JS_CCH_H