#ifndef STATIC_LINK_VW
#define BOOST_TEST_DYN_LINK
#endif

#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>

#include "test_common.h"

#include <vector>
#include "conditional_contextual_bandit.h"
#include "vw.h"
// TODO: Make unit test dig out and verify features.
BOOST_AUTO_TEST_CASE(parse_json_simple)
{
  auto vw = VW::initialize("--json --no_stdin --quiet", nullptr, false, nullptr, nullptr);

  std::string json_text = R"(
    {
      "_label": 1,
      "features": {
        "13": 3.9656971e-02,
        "24303": 2.2660980e-01,
        "const": 0.01
      }
    })";

  auto examples = parse_json(*vw, json_text);

  BOOST_CHECK_EQUAL(examples.size(), 1);
  BOOST_CHECK_CLOSE(examples[0]->l.simple.label, 1.f, FLOAT_TOL);
  VW::finish_example(*vw, examples);
  VW::finish(*vw);
}

// TODO: Make unit test dig out and verify features.
BOOST_AUTO_TEST_CASE(parse_json_cb)
{
  std::string json_text = R"(
    {
      "s_": "1",
      "s_": "2",
      "_labelIndex": 0,
      "_label_Action": 1,
      "_label_Cost": 1,
      "_label_Probability": 0.5,
      "_multi": [
        {
          "a_": "1",
          "b_": "1",
          "c_": "1"
        },
        {
          "a_": "2",
          "b_": "2",
          "c_": "2"
        },
        {
          "a_": "3",
          "b_": "3",
          "c_": "3"
        }
      ]
    })";

  auto vw = VW::initialize("--cb_adf --json --no_stdin --quiet", nullptr, false, nullptr, nullptr);
  auto examples = parse_json(*vw, json_text);
  BOOST_CHECK_EQUAL(examples.size(), 4);

  BOOST_CHECK_EQUAL(examples[0]->l.cb.costs.size(), 1);
  BOOST_CHECK_CLOSE(examples[0]->l.cb.costs[0].probability, -1.f, FLOAT_TOL);
  BOOST_CHECK_CLOSE(examples[0]->l.cb.costs[0].cost, FLT_MAX, FLOAT_TOL);

  // Action examples
  BOOST_CHECK_EQUAL(examples[1]->l.cb.costs.size(), 1);
  BOOST_CHECK_EQUAL(examples[2]->l.cb.costs.size(), 0);
  BOOST_CHECK_EQUAL(examples[3]->l.cb.costs.size(), 0);

  BOOST_CHECK_CLOSE(examples[1]->l.cb.costs[0].probability, 0.5, FLOAT_TOL);
  BOOST_CHECK_CLOSE(examples[1]->l.cb.costs[0].cost, 1.0, FLOAT_TOL);
  BOOST_CHECK_EQUAL(examples[1]->l.cb.costs[0].action, 1);
  VW::finish_example(*vw, examples);
  VW::finish(*vw);
}

// TODO: Make unit test dig out and verify features.
BOOST_AUTO_TEST_CASE(parse_json_ccb)
{
  std::string json_text = R"(
    {
      "s_": "1",
      "_multi": [
        {
          "b_": "1",
          "c_": "1",
          "d_": "1"
        },
        {
          "b_": "2",
          "c_": "2",
          "d_": "2"
        },
        {
          "b_": "2",
          "c_": "2",
          "d_": "2"
        },
        {
          "b_": "2",
          "c_": "2",
          "d_": "2"
        }
      ],
      "_slots": [
        {
          "_id": "00eef1eb-2205-4f47",
          "_inc": [1,2],
          "test": 4,
          "_label_cost": 2,
          "_o": [],
          "_a": 1,
          "_p": 0.25
        },
        {
          "other_feature": 3
        },
        {
          "_id": "set_id",
          "other": 6,
          "_label_cost": 4,
          "_o": [],
          "_a": [2,1],
          "_p": [0.75,0.25]
        }
      ]
    })";

  auto vw = VW::initialize("--ccb_explore_adf --json --no_stdin --quiet", nullptr, false, nullptr, nullptr);

  auto examples = parse_json(*vw, json_text);

  BOOST_CHECK_EQUAL(examples.size(), 8);
  BOOST_CHECK_EQUAL(examples[0]->l.conditional_contextual_bandit.type, CCB::example_type::shared);
  BOOST_CHECK_EQUAL(examples[1]->l.conditional_contextual_bandit.type, CCB::example_type::action);
  BOOST_CHECK_EQUAL(examples[2]->l.conditional_contextual_bandit.type, CCB::example_type::action);
  BOOST_CHECK_EQUAL(examples[3]->l.conditional_contextual_bandit.type, CCB::example_type::action);
  BOOST_CHECK_EQUAL(examples[4]->l.conditional_contextual_bandit.type, CCB::example_type::action);
  BOOST_CHECK_EQUAL(examples[5]->l.conditional_contextual_bandit.type, CCB::example_type::slot);
  BOOST_CHECK_EQUAL(examples[6]->l.conditional_contextual_bandit.type, CCB::example_type::slot);
  BOOST_CHECK_EQUAL(examples[7]->l.conditional_contextual_bandit.type, CCB::example_type::slot);

  auto label1 = examples[5]->l.conditional_contextual_bandit;
  BOOST_CHECK_EQUAL(label1.explicit_included_actions.size(), 2);
  BOOST_CHECK_EQUAL(label1.explicit_included_actions[0], 1);
  BOOST_CHECK_EQUAL(label1.explicit_included_actions[1], 2);
  BOOST_CHECK_CLOSE(label1.outcome->cost, 2.f, .0001f);
  BOOST_CHECK_EQUAL(label1.outcome->probabilities.size(), 1);
  BOOST_CHECK_EQUAL(label1.outcome->probabilities[0].action, 1);
  BOOST_CHECK_CLOSE(label1.outcome->probabilities[0].score, .25f, .0001f);

  auto label2 = examples[6]->l.conditional_contextual_bandit;
  BOOST_CHECK_EQUAL(label2.explicit_included_actions.size(), 0);
  BOOST_CHECK(label2.outcome == nullptr);

  auto label3 = examples[7]->l.conditional_contextual_bandit;
  BOOST_CHECK_EQUAL(label3.explicit_included_actions.size(), 0);
  BOOST_CHECK_CLOSE(label3.outcome->cost, 4.f, .0001f);
  BOOST_CHECK_EQUAL(label3.outcome->probabilities.size(), 2);
  BOOST_CHECK_EQUAL(label3.outcome->probabilities[0].action, 2);
  BOOST_CHECK_CLOSE(label3.outcome->probabilities[0].score, .75f, .0001f);
  BOOST_CHECK_EQUAL(label3.outcome->probabilities[1].action, 1);
  BOOST_CHECK_CLOSE(label3.outcome->probabilities[1].score, .25f, .0001f);
  VW::finish_example(*vw, examples);
  VW::finish(*vw);
}

BOOST_AUTO_TEST_CASE(parse_json_cb_as_ccb)
{
  std::string json_text = R"(
    {
      "s_": "1",
      "s_": "2",
      "_labelIndex": 0,
      "_label_Action": 1,
      "_label_Cost": 1,
      "_label_Probability": 0.5,
      "_multi": [
        {
          "a_": "1",
          "b_": "1",
          "c_": "1"
        },
        {
          "a_": "2",
          "b_": "2",
          "c_": "2"
        },
        {
          "a_": "3",
          "b_": "3",
          "c_": "3"
        }
      ]
    })";

  auto vw = VW::initialize("--ccb_explore_adf --json --no_stdin --quiet", nullptr, false, nullptr, nullptr);

  auto examples = parse_json(*vw, json_text);

  BOOST_CHECK_EQUAL(examples.size(), 5);
  BOOST_CHECK_EQUAL(examples[0]->l.conditional_contextual_bandit.type, CCB::example_type::shared);
  BOOST_CHECK_EQUAL(examples[1]->l.conditional_contextual_bandit.type, CCB::example_type::action);
  BOOST_CHECK_EQUAL(examples[2]->l.conditional_contextual_bandit.type, CCB::example_type::action);
  BOOST_CHECK_EQUAL(examples[3]->l.conditional_contextual_bandit.type, CCB::example_type::action);
  BOOST_CHECK_EQUAL(examples[4]->l.conditional_contextual_bandit.type, CCB::example_type::slot);

  auto label1 = examples[4]->l.conditional_contextual_bandit;
  BOOST_CHECK_EQUAL(label1.explicit_included_actions.size(), 0);
  BOOST_CHECK_CLOSE(label1.outcome->cost, 1.f, .0001f);
  BOOST_CHECK_EQUAL(label1.outcome->probabilities.size(), 1);
  BOOST_CHECK_EQUAL(label1.outcome->probabilities[0].action, 1);
  BOOST_CHECK_CLOSE(label1.outcome->probabilities[0].score, .5f, .0001f);
  VW::finish_example(*vw, examples);
  VW::finish(*vw);
}


BOOST_AUTO_TEST_CASE(parse_json_slates_dom_parser)
{
  std::string json_text = R"(
{
    "GUser": {
        "id": "mk",
        "major": "psychology",
        "hobby": "kids",
        "favorite_character": "7of9"
    },
    "_multi": [
        {
            "_slot_id": 0,
            "TAction": {
                "topic": "SkiConditions-VT"
            }
        },
        {
            "_slot_id": 0,
            "TAction": {
                "topic": "HerbGarden"
            }
        },
        {
            "_slot_id": 1,
            "TAction": {
                "topic": "BeyBlades"
            }
        },
        {
            "_slot_id": 1,
            "TAction": {
                "topic": "NYCLiving"
            }
        },
        {
            "_slot_id": 1,
            "TAction": {
                "topic": "MachineLearning"
            }
        }
    ],
    "_slots": [
        {
            "slot_id": "__0"
        },
        {
            "slot_id": "__2"
        }
    ]
}
)";

  // Assert parsed values against what they should be
  auto slates_vw = VW::initialize("--slates --dsjson --no_stdin --quiet", nullptr, false, nullptr, nullptr);
  auto examples = parse_json(*slates_vw, json_text);

  BOOST_CHECK_EQUAL(examples.size(), 8);
  BOOST_CHECK_EQUAL(examples[0]->l.slates.type, VW::slates::example_type::shared);
  BOOST_CHECK_EQUAL(examples[1]->l.slates.type, VW::slates::example_type::action);
  BOOST_CHECK_EQUAL(examples[2]->l.slates.type, VW::slates::example_type::action);
  BOOST_CHECK_EQUAL(examples[3]->l.slates.type, VW::slates::example_type::action);
  BOOST_CHECK_EQUAL(examples[4]->l.slates.type, VW::slates::example_type::action);
  BOOST_CHECK_EQUAL(examples[5]->l.slates.type, VW::slates::example_type::action);
  BOOST_CHECK_EQUAL(examples[6]->l.slates.type, VW::slates::example_type::slot);
  BOOST_CHECK_EQUAL(examples[7]->l.slates.type, VW::slates::example_type::slot);

  const auto& label0 = examples[0]->l.slates;
  BOOST_CHECK_EQUAL(label0.labeled, false);
  BOOST_CHECK_EQUAL(examples[1]->l.slates.slot_id, 0);
  BOOST_CHECK_EQUAL(examples[2]->l.slates.slot_id, 0);
  BOOST_CHECK_EQUAL(examples[3]->l.slates.slot_id, 1);
  BOOST_CHECK_EQUAL(examples[4]->l.slates.slot_id, 1);
  BOOST_CHECK_EQUAL(examples[5]->l.slates.slot_id, 1);

  check_collections_exact(examples[0]->indices, std::vector<namespace_index>{'G'});
  BOOST_CHECK_EQUAL(examples[0]->feature_space['G'].indicies.size(), 4);

  VW::finish_example(*slates_vw, examples);
  VW::finish(*slates_vw);
}
