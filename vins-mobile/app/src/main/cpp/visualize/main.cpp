#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <GLES3/gl3.h>
#include <cstdio>
#include <iostream>

#include "utils.hpp"
#include "application.hpp"

#include <ros/ros.h>
#include <nav_msgs/Path.h>
#include <sensor_msgs/PointCloud.h>

application app(1280, 720);

double prev_cursor_pos_x, prev_cursor_pos_y;

static void glfwCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (state == GLFW_PRESS){
        double dx = xpos - prev_cursor_pos_x;
        double dy = ypos - prev_cursor_pos_y;

        // std::cout<<dx<<" "<<dy<<"\n";
        app.cameraRotation(dx, dy);

        prev_cursor_pos_x = xpos;
        prev_cursor_pos_y = ypos;
    }
}

void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        glfwGetCursorPos(window, &prev_cursor_pos_x, &prev_cursor_pos_y);
}

void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    app.cameraDistance(yoffset);
}

void pathSubscribeCallback(nav_msgs::PathConstPtr path) {
    GLfloat *vertices = new GLfloat[path->poses.size()*3];
    GLuint *indices = new GLuint[path->poses.size()*2 - 2];
    int j=0;
    for(int i=0; i<path->poses.size(); i++) {
        vertices[i*3] = path->poses.at(i).pose.position.x;
        vertices[i*3+1] = path->poses.at(i).pose.position.y;
        vertices[i*3+2] = path->poses.at(i).pose.position.z;
        // vertices[i*3+2] = path->poses.at(i).pose.position.z;

        if(j >= path->poses.size()*2 - 2) continue;
        indices[j++] = i;
        indices[j++] = i+1;
    }
    // std::cout<<vertices[path->poses.size()*3-3]<<" "<<vertices[path->poses.size()*3-2]<<" "<<vertices[path->poses.size()*3-1]<<"\n";
    app.updateVertex(1, vertices, path->poses.size()*3);
    app.updateIndex(1, indices, path->poses.size()*2 - 2);
}

void pointcloudSubscribeCallback(sensor_msgs::PointCloudConstPtr pc) {
    GLfloat *vertices = new GLfloat[pc->points.size()*3];
    GLuint *indices = new GLuint[pc->points.size()];
    for (int i=0; i<pc->points.size(); i++) {
        vertices[i*3] = pc->points.at(i).x;
        vertices[i*3+1] = pc->points.at(i).y;
        vertices[i*3+2] = pc->points.at(i).z;
        
        indices[i] = i;
    }
    app.updateVertex(2, vertices, pc->points.size()*3);
    app.updateIndex(2, indices, pc->points.size());
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "visualize_node");

    ros::NodeHandle nodeHandler;
	ros::Subscriber pathSubscriber = nodeHandler.subscribe("/pose_graph/path_1", 1, pathSubscribeCallback);
	ros::Subscriber pointSubscriber = nodeHandler.subscribe("/vins_estimator/point_cloud", 1, pointcloudSubscribeCallback);
	
	GLFWwindow* window;
    
    // Initialize the library
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    
    // set mouse event
    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    glfwSetCursorPosCallback(window, glfwCursorPositionCallback);
    glfwSetMouseButtonCallback(window, glfwMouseButtonCallback);
    glfwSetScrollCallback(window, glfwScrollCallback);
	
    // Make the window's context current 
    glfwMakeContextCurrent(window);

	app.init();

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        ros::spinOnce();

		app.draw();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
	}

	app.shutdown();
    glfwTerminate();
	return 0;
}