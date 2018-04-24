
#include <marker_publishing_utils.h>

namespace rviz_interface
{
    void init_point(geometry_msgs::Point & point, float x, float y, float z)
    {
        point.x = x;
        point.y = y;
        point.z = z;
    }

    void push_segment(visualization_msgs::Marker & marker, geometry_msgs::Point & start, geometry_msgs::Point & end)
    {
        marker.points.push_back(start);
        marker.points.push_back(end);
    }

    void build_cube_wire(visualization_msgs::Marker & marker, octomath::Vector3 const& geofence_min, octomath::Vector3 const& geofence_max, ros::Publisher const& marker_pub, octomath::Vector3 color = octomath::Vector3(1, 1, 1))
    {
        uint32_t shape = visualization_msgs::Marker::LINE_LIST; 
        // Set the frame ID and timestamp.  See the TF tutorials for information on these. 
        marker.header.frame_id = "/map"; 
        marker.header.stamp = ros::Time::now(); 
        marker.type = shape; 
        marker.action = visualization_msgs::Marker::ADD; 
        marker.scale.x = 0.2; 
        marker.scale.y = 0.2; 
        marker.scale.z = 0.2; 
        marker.color.r = color.x(); 
        marker.color.g = color.y(); 
        marker.color.b = color.z(); 
        marker.color.a = 1.0; 
        geometry_msgs::Point A, B, C, D, E, F, G, H; 
        init_point( A, geofence_min.x(), geofence_min.y(), geofence_min.z()); 
        init_point( B, geofence_max.x(), geofence_min.y(), geofence_min.z()); 
        init_point( C, geofence_min.x(), geofence_max.y(), geofence_min.z()); 
        init_point( D, geofence_max.x(), geofence_max.y(), geofence_min.z()); 
        init_point( E, geofence_min.x(), geofence_max.y(), geofence_max.z()); 
        init_point( F, geofence_max.x(), geofence_max.y(), geofence_max.z()); 
        init_point( G, geofence_min.x(), geofence_min.y(), geofence_max.z()); 
        init_point( H, geofence_max.x(), geofence_min.y(), geofence_max.z()); 
        push_segment(marker, A, B); 
        push_segment(marker, A, G); 
        push_segment(marker, A, C); 
        push_segment(marker, B, H); 
        push_segment(marker, B, D); 
        push_segment(marker, G, H); 
        push_segment(marker, H, F); 
        push_segment(marker, C, D); 
        push_segment(marker, C, E); 
        push_segment(marker, F, D); 
        push_segment(marker, G, E); 
        push_segment(marker, E, F); 
    }

	void publish_geofence(octomath::Vector3 const& geofence_min, octomath::Vector3 const& geofence_max, ros::Publisher const& marker_pub) 
    { 
        visualization_msgs::Marker marker; 
        // Set the frame ID and timestamp.  See the TF tutorials for information on these. 
        marker.ns = "geofence"; 
        marker.id = 20; 
        marker.lifetime = ros::Duration(); 
        build_cube_wire(marker, geofence_min, geofence_max, marker_pub);
        marker_pub.publish(marker); 
    } 

    void publish_marker_safety_margin(geometry_msgs::Point const& frontier, double safety_margin, ros::Publisher const& marker_pub, int id)
    {   
        visualization_msgs::Marker marker;
        octomath::Vector3  max = octomath::Vector3(frontier.x - safety_margin, frontier.y - safety_margin, frontier.z - safety_margin);
        octomath::Vector3  min = octomath::Vector3(frontier.x + safety_margin, frontier.y + safety_margin, frontier.z + safety_margin);
        marker.lifetime = ros::Duration();
        marker.ns = "frontier_safety_margin";
        marker.id = id;
        build_cube_wire(marker, min, max, marker_pub);
        visualization_msgs::MarkerArray marker_array;
        marker_array.markers.push_back(marker);
        marker_pub.publish(marker_array); 
    }

    void publish_deleteAll(ros::Publisher const& marker_pub)
    {
        visualization_msgs::MarkerArray marker_array;
        visualization_msgs::Marker marker;
        marker.action = visualization_msgs::Marker::DELETEALL;
        marker_array.markers.push_back(marker);
        marker_pub.publish(marker_array); 
    }

    void publish_voxel_free_occupied(octomath::Vector3 & candidate, bool is_occupied, ros::Publisher const& marker_pub, int id, double size, visualization_msgs::Marker & marker)
    {
        uint32_t shape = visualization_msgs::Marker::CUBE;
        // Set the frame ID and timestamp.  See the TF tutorials for information on these.
        marker.header.frame_id = "/map";
        marker.header.stamp = ros::Time::now();
        marker.ns = "frontier_neighborhood";
        marker.id = id;
        marker.type = shape;
        marker.action = visualization_msgs::Marker::ADD;
        marker.pose.position.x = candidate.x();
        marker.pose.position.y = candidate.y();
        marker.pose.position.z = candidate.z();
        marker.pose.orientation.x = 0.0;
        marker.pose.orientation.y = 0.0;
        marker.pose.orientation.z = 0.0;
        marker.pose.orientation.w = 1.0;
        marker.scale.x = size;
        marker.scale.y = size;
        marker.scale.z = size;
        if(is_occupied)
        {
            marker.color.r = 1.0f;
            marker.color.g = 0.0f;
            marker.color.b = 0.0f;
            marker.color.a = 0.8;
        }
        else
        {
            marker.color.r = 0.0f;
            marker.color.g = 1.0f;
            marker.color.b = 0.0f;
            marker.color.a = 0.8;
        }
        marker.lifetime = ros::Duration(4);
    }

    void publish_small_marker(octomath::Vector3 const& candidate, ros::Publisher const& marker_pub, float red, float green, float blue, std::string ns, int id)
    {
        uint32_t shape = visualization_msgs::Marker::CUBE;
        visualization_msgs::Marker marker;
        // Set the frame ID and timestamp.  See the TF tutorials for information on these.
        marker.header.frame_id = "/map";
        marker.header.stamp = ros::Time::now();
        marker.ns = ns;
        marker.id = id;
        marker.type = shape;
        marker.action = visualization_msgs::Marker::ADD;
        marker.pose.position.x = candidate.x();
        marker.pose.position.y = candidate.y();
        marker.pose.position.z = candidate.z();
        marker.pose.orientation.x = 0.0;
        marker.pose.orientation.y = 0.0;
        marker.pose.orientation.z = 0.0;
        marker.pose.orientation.w = 1.0;
        marker.scale.x = 0.2f;
        marker.scale.y = 0.2f;
        marker.scale.z = 0.2f;
        marker.color.r = red;
        marker.color.g = green;
        marker.color.b = blue;
        marker.color.a = 1;
    
        marker.lifetime = ros::Duration(7);
        marker_pub.publish(marker);
    }

    void publish_current_position(octomath::Vector3 & candidate, ros::Publisher const& marker_pub)
    {
        float red = 0.0f;
        float green = 1.0f;
        float blue = 1.0f;
        publish_small_marker(candidate, marker_pub,red,  green,  blue, "current_position", 21);
    }

    void publish_start(geometry_msgs::Point const& candidate, ros::Publisher const& marker_pub)
    {
        octomath::Vector3 candidate_vec3 (candidate.x, candidate.y, candidate.z);
        float red = 1.0f;
        float green = 1.0f;
        float blue = 0.0f;
        publish_small_marker(candidate_vec3, marker_pub,red,  green,  blue, "start", 22);
    }

    void publish_goal(geometry_msgs::Point const& candidate, ros::Publisher const& marker_pub)
    {
        octomath::Vector3 candidate_vec3 (candidate.x, candidate.y, candidate.z);
        float red = 1.0f;
        float green = 0.5f;
        float blue = 0.0f;
        publish_small_marker(candidate_vec3, marker_pub,red,  green,  blue, "goal", 23);
    }

    void publish_sensing_position(octomath::Vector3 const& position, ros::Publisher const& marker_pub)
    {
        float red = 0.0f;
        float green = 0.0f;
        float blue = 0.25f;
        publish_small_marker(position, marker_pub,red,  green,  blue, "sensing_position", 24);
    }

    void publish_frontier_marker(geometry_msgs::Point const& candidate, bool is_frontier, ros::Publisher const& marker_pub)
    {
        octomath::Vector3 candidate_vec3 (candidate.x, candidate.y, candidate.z);
        publish_frontier_marker(candidate_vec3, is_frontier, marker_pub);
    }
    
    void publish_frontier_marker(octomath::Vector3 const& candidate, bool is_frontier, ros::Publisher const& marker_pub)
    {
        uint32_t shape = visualization_msgs::Marker::CUBE;
        visualization_msgs::Marker marker;
        // Set the frame ID and timestamp.  See the TF tutorials for information on these.
        marker.header.frame_id = "/map";
        marker.header.stamp = ros::Time::now();
        marker.ns = "frontier_candidate";
        marker.id = 10;
        marker.type = shape;
        marker.action = visualization_msgs::Marker::ADD;
        marker.pose.position.x = candidate.x();
        marker.pose.position.y = candidate.y();
        marker.pose.position.z = candidate.z();
        marker.pose.orientation.x = 0.0;
        marker.pose.orientation.y = 0.0;
        marker.pose.orientation.z = 0.0;
        marker.pose.orientation.w = 1.0;
        marker.scale.x = 0.2;
        marker.scale.y = 0.2;
        marker.scale.z = 0.2;
        if(is_frontier)
        {
            marker.color.r = 0.0f;
            marker.color.g = 0.0f;
            marker.color.b = 1.0f;
            marker.color.a = 1.0;
        }
        else
        {
            marker.color.r = 0.5f;
            marker.color.g = 0.5f;
            marker.color.b = 0.75f;
            marker.color.a = 1.0;
        }
        marker.lifetime = ros::Duration();
        // ROS_WARN_STREAM("[RVIZ PUB] Frontier at " << marker.pose.position << ". Color: " << marker.color.r << ", " << marker.color.g << ", " << marker.color.b);
        marker_pub.publish(marker);
    }

    void build_arrow_path(octomath::Vector3 & start, octomath::Vector3 & goal, int request_id, visualization_msgs::Marker & marker)
    {
        uint32_t shape = visualization_msgs::Marker::ARROW;
        // Set the frame ID and timestamp.  See the TF tutorials for information on these.
        marker.header.frame_id = "/map";
        marker.header.stamp = ros::Time::now();
        marker.ns = "lazy_theta_star_path";
        marker.id = request_id;
        marker.type = shape;
        geometry_msgs::Point goal_point;
        goal_point.x = goal.x();
        goal_point.y = goal.y();
        goal_point.z = goal.z();
        marker.points.push_back(goal_point);
        marker.action = visualization_msgs::Marker::ADD;
        geometry_msgs::Point start_point;
        start_point.x = start.x();
        start_point.y = start.y();
        start_point.z = start.z();
        marker.points.push_back(start_point);
        marker.pose.orientation.w = 1.0;
        marker.scale.x = 0.1;
        marker.scale.y = 0.3;
        marker.scale.z = 0;
        marker.color.r = 200;
        marker.color.g = 100;
        marker.color.b = 0;
        marker.color.a = 1;
        
        marker.lifetime = ros::Duration();
    }

    void build_waypoint(octomath::Vector3 & candidate, double size, int color, int waypoint_id, visualization_msgs::Marker & marker)
    {   
        uint32_t shape = visualization_msgs::Marker::CUBE;
        // Set the frame ID and timestamp.  See the TF tutorials for information on these.
        marker.header.frame_id = "/map";
        marker.header.stamp = ros::Time::now();
        marker.ns = "lazy_theta_star_waypoint";
        marker.id = waypoint_id;
        marker.type = shape;
        marker.action = visualization_msgs::Marker::ADD;
        marker.pose.position.x = candidate.x();
        marker.pose.position.y = candidate.y();
        marker.pose.position.z = candidate.z();
        marker.pose.orientation.w = 1.0;
        marker.scale.x = size;
        marker.scale.y = size;
        marker.scale.z = size;
        marker.color.r = 0.9f;
        marker.color.g = color+0.4;
        marker.color.b = 1.0f;
        // ROS_WARN_STREAM("[RVIZ PUB] color " << marker.color.r << ", " << marker.color.g << ", " << marker.color.b << " i: " << waypoint_id);
        marker.color.a = 0.8;
        
        marker.lifetime = ros::Duration();
    }
}