
// Copyright (c) by respective owners including Yahoo!, Microsoft, and
// individual contributors. All rights reserved. Released under a BSD (revised)
// license as described in the file LICENSE.
#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef uint32_t vw_status;
static const vw_status vw_success = 0;
static const vw_status vw_fail = 1;
static const vw_status vw_not_implemented = 2;

typedef uint32_t vw_label_type;
static const vw_label_type simple = 0;
static const vw_label_type cb = 1;
static const vw_label_type cb_eval = 2;
static const vw_label_type cs = 3;
static const vw_label_type multi = 4;
static const vw_label_type mc = 5;
static const vw_label_type ccb = 6;
static const vw_label_type slates = 7;

typedef uint32_t vw_prediction_type;
static const vw_prediction_type scalar = 0;
static const vw_prediction_type scalars = 1;
static const vw_prediction_type action_scores = 2;
static const vw_prediction_type action_probs = 3;
static const vw_prediction_type multiclass = 4;
static const vw_prediction_type multilabels = 5;
static const vw_prediction_type prob = 6;
static const vw_prediction_type multiclassprobs = 7;
static const vw_prediction_type decision_probs = 8;

struct vw_workspace_tag;
typedef struct vw_workspace_tag vw_workspace;

struct vw_label_tag;
typedef struct vw_label_tag vw_label;

struct vw_prediction_tag;
typedef struct vw_prediction_tag vw_prediction;

struct vw_example_tag;
typedef struct vw_example_tag vw_example;

struct vw_interactions_tag;
typedef struct vw_interactions_tag vw_interactions;

struct vw_feature_space_tag;
typedef struct vw_feature_space_tag vw_feature_space;

struct vw_feature_space_ref_tag;
typedef struct vw_feature_space_ref_tag vw_feature_space_ref;

struct vw_decision_scores_tag;
typedef struct vw_decision_scores_tag vw_decision_scores;

struct vw_simple_label_tag;
typedef struct vw_simple_label_tag vw_simple_label;

struct vw_multiclass_label_tag;
typedef struct vw_multiclass_label_tag vw_multiclass_label;

struct vw_cs_label_tag;
typedef struct vw_cs_label_tag vw_cs_label;

struct vw_cb_label_tag;
typedef struct vw_cb_label_tag vw_cb_label;

struct vw_ccb_label_tag;
typedef struct vw_ccb_label_tag vw_ccb_label;

struct vw_ccb_outcome_tag;
typedef struct vw_ccb_outcome_tag vw_ccb_outcome;

struct vw_slates_label_tag;
typedef struct vw_slates_label_tag vw_slates_label;

struct vw_cb_eval_label_tag;
typedef struct vw_cb_eval_label_tag vw_cb_eval_label;

struct vw_multilabels_label_tag;
typedef struct vw_multilabels_label_tag vw_multilabels_label;

#ifdef __cplusplus
}
#endif

//   typedef void* VW_HANDLE;
//   typedef void* VW_EXAMPLE;
//   typedef void* VW_LABEL;
//   typedef void* VW_FEATURE_SPACE;
//   typedef void* VW_FEATURE;
//   typedef void* VW_IOBUF;