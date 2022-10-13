# Project status

[Back](./README.md)

| Task           | Current Status | Finished | TRACKER |
|----------------|---------------|---------------|---------------|
| Create a readme | Done with limiations | NO | https://github.com/AndreiTurean/measurement_data_solution/issues/17
| Create project configuration   | IN WORK | NO | https://github.com/AndreiTurean/measurement_data_solution/issues/17
| Create interface documentation  | DONE | YES | https://github.com/AndreiTurean/measurement_data_solution/issues/17
| Create design documents for the components   | NEW | NO | N/A
| Create a backlog for better tracking  | DONE | YES | https://github.com/users/AndreiTurean/projects/1

## Legend

| Current status | Value meaning |
|----------------|---------------|
| Done | The task was done and review-ed
| Done with limitations   | The task was done and delivered partially, being followed up by another subtask
| IN WORK   | The task is on work
| NEW   | The task was created, but need to be analysed before deveopment is started.

# IMPORTANT

All the tasks will be monitored via the [backlog](https://github.com/users/AndreiTurean/projects/1) of the project.

## Limitations

| Limitation | Comment |
|----------------|---------------|
| No real data aquisition is possible. | In the current version of the MDS project there is no data aquistion, but there is a plan to perform data aquistion from camera devices using the openCV library. This will be done in issue: https://github.com/AndreiTurean/measurement_data_solution/issues/10
| CLI is not implemented | The CLI version of the MDS is not currently implemented, but it will be released when a template configuration file issue will be finished: https://github.com/AndreiTurean/measurement_data_solution/issues/15
| Lack of UNIT test and MODULE test | There is a lack of code coverage that will be later patched in the issue: https://github.com/AndreiTurean/measurement_data_solution/issues/16
| The project is still a demo | The project is still a demo and was not tested on a real setup. Currently it got some architectural changes and the project future is not clear yet.
| Lack of documentation | The project lacks documentation and the code as well. It will be resolved over the time, but in the current scenario it is very hard to understand the project using the provided documentation.
| The memory and CPU profile is not consistent | The memory and CPU profiling of the project is done extremly rare.
| Existing unit tests are not reliable on release preset | The unit tests are not reliable on release preset because the SHARED object name is hardcoded and the debug object has the postfix "_debug"