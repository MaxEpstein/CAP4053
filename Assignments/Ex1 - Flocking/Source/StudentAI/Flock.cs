using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using AI.SteeringBehaviors.Core;

namespace AI.SteeringBehaviors.StudentAI
{
    public class Flock
    {
        public float AlignmentStrength { get; set; }
        public float CohesionStrength { get; set; }
        public float SeparationStrength { get; set; }
        public List<MovingObject> Boids { get; protected set; }
        public Vector3 AveragePosition { get; set; }
        protected Vector3 AverageForward { get; set; }
        public float FlockRadius { get; set; }

        #region TODO
        private void calcAvgPosition()
        {
            float avgX = 0, avgY = 0, avgZ = 0;
            foreach (MovingObject obj in Boids)
            {
                avgX += obj.Position.X;
                avgY += obj.Position.Y;
                avgZ += obj.Position.Z;
            }

            avgX /= Boids.Count;
            avgY /= Boids.Count;
            avgZ /= Boids.Count;

            Vector3 avgPosition = new Vector3(avgX, avgY, avgZ);

            AveragePosition = avgPosition;
        }

        private void calcAvgForward()
        {
            float avgX = 0, avgY = 0, avgZ = 0;
            foreach (MovingObject obj in Boids)
            {
                avgX += obj.Velocity.X;
                avgY += obj.Velocity.Y;
                avgZ += obj.Velocity.Z;
            }

            avgX /= Boids.Count;
            avgY /= Boids.Count;
            avgZ /= Boids.Count;

            Vector3 avgForw = new Vector3(avgX, avgY, avgZ);

            AverageForward = avgForw;
        }
        private Vector3 calc_alignment_accel(MovingObject obj)
        {
            Vector3 accel = AverageForward / obj.MaxSpeed;

            if (accel.Length > 1.0)
            {
                accel.Normalize();
            }

            return accel * AlignmentStrength;
        }

        private Vector3 calc_cohesion_accel(MovingObject obj)
        {
            Vector3 accel = AveragePosition - obj.Position;
            float dist = accel.Length;
            accel.Normalize();

            if (dist < FlockRadius)
            {
                accel = accel * dist / FlockRadius;
            }

            return accel * CohesionStrength;
        }

        private Vector3 calc_separation_accel(MovingObject obj)
        {
            Vector3 sum = Vector3.Zero;
            foreach (MovingObject sibling in Boids)
            {
                if (obj == sibling)
                {
                    continue;
                }

                Vector3 accel = obj.Position - sibling.Position;
                float dist = accel.Length;
                float safeDist = obj.SafeRadius + sibling.SafeRadius;

                if (dist < safeDist)
                {
                    accel.Normalize();
                    accel = accel * (safeDist - dist) / safeDist;
                    sum = sum + accel;
                }
            }

            if (sum.Length > 1.0)
            {
                sum.Normalize();
            }

            return sum * SeparationStrength;
        }
        public virtual void Update(float deltaTime)
        {
            calcAvgForward();
            calcAvgPosition();

            foreach (MovingObject obj in Boids)
            {
                Vector3 accel = calc_alignment_accel(obj);
                accel = accel + calc_cohesion_accel(obj);
                accel = accel + calc_separation_accel(obj);
                accel = accel * obj.MaxSpeed * deltaTime;
                obj.Velocity = obj.Velocity + accel;

                if (obj.Velocity.Length > obj.MaxSpeed)
                {
                    obj.Velocity.Normalize();
                    obj.Velocity = obj.Velocity * obj.MaxSpeed;
                }

                obj.Update(deltaTime);
            }
        }
        #endregion
    }
}
