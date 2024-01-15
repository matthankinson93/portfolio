import { Component, OnInit } from '@angular/core';
import { NgForm } from '@angular/forms';
import { MatDialog } from '@angular/material/dialog';
import { DeviceCreateDialogComponent } from '../device-create-dialog/device-create-dialog.component';
import { UserInviteDialogComponent } from '../user-invite-dialog/user-invite-dialog.component';
import { DeviceService } from '../services/device.service';
import { Device } from '../models/device.model';
import { UserService } from '../services/user.service';
import { User } from '../models/user.model';
import { DeviceDetailDialogComponent } from '../device-detail-dialog/device-detail-dialog.component';

@Component({
  selector: 'app-overview',
  templateUrl: './overview.component.html',
  styleUrls: ['./overview.component.css']
})
export class OverviewComponent implements OnInit {
  devices: Device[] = [];
  users: User[] = [];
  newUser: User = { email: ''};

  constructor(
    public dialog: MatDialog,
    private deviceService: DeviceService,
    private userService: UserService
  ) {}

  ngOnInit() {
    this.loadUsers();
    this.getDevices();
  }

  private loadUsers(): void {
    this.userService.getAllUsers().subscribe(
      (data: User[]) => {
        this.users = [...data];
      },
      error => {
        console.error('There was an error fetching the users', error);
      }
    );
  }

  inviteUser(form: NgForm): void {
    if (form.valid) {
      this.userService.createUser(this.newUser).subscribe({
        next: (user) => {
          this.newUser = { email: '' };
          form.resetForm(); 
        },
        error: (error) => {
          console.error('Error inviting user:', error);
        }
      });
    }
  }

  getDevices(): void {
    this.deviceService.getAllDevices().subscribe({
      next: (data) => {
        if (data && data.length) {
          this.devices = data;
        } else {
          this.devices = [{
            name: "No devices available...",
            owner_id: ""
          }];
        }
      },
      error: (err) => {
        console.error('There was an error fetching the devices:', err);
      }
    });
  }

  openCreateDeviceDialog(): void {
    const dialogRef = this.dialog.open(DeviceCreateDialogComponent, {});
    dialogRef.afterClosed().subscribe(result => {
      this.getDevices();
    });
  }

  openInviteUserDialog(): void {
    this.dialog.open(UserInviteDialogComponent, {});
  }

  openDetailDialog(device: Device): void {
    this.dialog.open(DeviceDetailDialogComponent, {
      data: { device: device }
    });
  }
}